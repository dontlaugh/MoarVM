#include "moar.h"
#include "platform/io.h"

#ifndef _WIN32
#include <sys/types.h>
#include <unistd.h>
#define DEFAULT_MODE 0x01B6
typedef struct stat STAT;
#else
#include <fcntl.h>
#define O_CREAT  _O_CREAT
#define O_RDONLY _O_RDONLY
#define O_WRONLY _O_WRONLY
#define O_TRUNC  _O_TRUNC
#define O_EXCL   _O_EXCL
#define O_RDWR   _O_RDWR
#define DEFAULT_MODE _S_IWRITE
#define open _open
#define close _close
#define read _read
#define write _write
#define isatty _isatty
#define ftruncate _chsize
#define fstat _fstat
typedef struct _stat STAT;
#endif

/* Data that we keep for a file-based handle. */
typedef struct {
    /* File descriptor. */
    int fd;

    /* The filename we opened, as a C string. */
    char *filename;
} MVMIOFileData;

/* Closes the file. */
static MVMint64 closefh(MVMThreadContext *tc, MVMOSHandle *h) {
    MVMIOFileData *data = (MVMIOFileData *)h->body.data;
    if (data->fd != -1) {
        int r = close(data->fd);
        data->fd = -1;
        if (r == -1)
            MVM_exception_throw_adhoc(tc, "Failed to close filehandle: %s", strerror(errno));
    }
    return 0;
}

/* Checks if the file is a TTY. */
static MVMint64 is_tty(MVMThreadContext *tc, MVMOSHandle *h) {
    MVMIOFileData *data = (MVMIOFileData *)h->body.data;
    return isatty(data->fd);
}

/* Gets the file descriptor. */
static MVMint64 mvm_fileno(MVMThreadContext *tc, MVMOSHandle *h) {
    MVMIOFileData *data = (MVMIOFileData *)h->body.data;
    return (MVMint64)data->fd;
}

/* Seek to the specified position in the file. */
static void seek(MVMThreadContext *tc, MVMOSHandle *h, MVMint64 offset, MVMint64 whence) {
    MVMIOFileData *data = (MVMIOFileData *)h->body.data;
    if (MVM_platform_lseek(data->fd, offset, whence) == -1)
        MVM_exception_throw_adhoc(tc, "Failed to seek in filehandle: %d", errno);
}

/* Get curernt position in the file. */
static MVMint64 mvm_tell(MVMThreadContext *tc, MVMOSHandle *h) {
    MVMIOFileData *data = (MVMIOFileData *)h->body.data;
    MVMint64 r;
    if ((r = MVM_platform_lseek(data->fd, 0, SEEK_CUR)) == -1)
        MVM_exception_throw_adhoc(tc, "Failed to tell in filehandle: %d", errno);
    return r;
}

/* Reads the specified number of bytes into a the supplied buffer, returning
 * the number actually read. */
static MVMint64 read_bytes(MVMThreadContext *tc, MVMOSHandle *h, char **buf_out, MVMint64 bytes) {
    MVMIOFileData *data = (MVMIOFileData *)h->body.data;
    char *buf = MVM_malloc(bytes);
    unsigned int interval_id = MVM_telemetry_interval_start(tc, "syncfile.read_to_buffer");
    MVMint32 bytes_read;
    MVM_gc_mark_thread_blocked(tc);
    if ((bytes_read = read(data->fd, buf, bytes)) == -1) {
        int save_errno = errno;
        MVM_free(buf);
        MVM_gc_mark_thread_unblocked(tc);
        MVM_exception_throw_adhoc(tc, "Reading from filehandle failed: %s",
            strerror(save_errno));
    }
    *buf_out = buf;
    MVM_gc_mark_thread_unblocked(tc);
    MVM_telemetry_interval_annotate(bytes_read, interval_id, "read this many bytes");
    MVM_telemetry_interval_stop(tc, interval_id, "syncfile.read_to_buffer");
    return bytes_read;
}

/* Checks if the end of file has been reached. */
static MVMint64 mvm_eof(MVMThreadContext *tc, MVMOSHandle *h) {
    MVMIOFileData *data = (MVMIOFileData *)h->body.data;
    MVMint64 seek_pos;
    STAT statbuf;
    if (fstat(data->fd, &statbuf) == -1)
        MVM_exception_throw_adhoc(tc, "Failed to stat file descriptor: %s",
            strerror(errno));
    if ((seek_pos = MVM_platform_lseek(data->fd, 0, SEEK_CUR)) == -1)
        MVM_exception_throw_adhoc(tc, "Failed to seek in filehandle: %d", errno);
    /* Comparison with seek_pos for some special files, like those in /proc,
     * which file size is 0 can be false. In that case, we fall back to check
     * file size to detect EOF. */
    return statbuf.st_size == seek_pos || statbuf.st_size == 0;
}

/* Writes the specified bytes to the file handle. */
static MVMint64 write_bytes(MVMThreadContext *tc, MVMOSHandle *h, char *buf, MVMint64 bytes) {
    MVMIOFileData *data = (MVMIOFileData *)h->body.data;
    MVMint64 bytes_written = 0;
    MVM_gc_mark_thread_blocked(tc);
    while (bytes > 0) {
        int r = write(data->fd, buf, (int)bytes);
        if (r == -1) {
            int save_errno = errno;
            MVM_gc_mark_thread_unblocked(tc);
            MVM_exception_throw_adhoc(tc, "Failed to write bytes to filehandle: %s",
                strerror(save_errno));
        }
        bytes_written += r;
        buf += r;
        bytes -= r;
    }
    MVM_gc_mark_thread_unblocked(tc);
    return bytes_written;
}

/* Flushes the file handle. */
static void flush(MVMThreadContext *tc, MVMOSHandle *h){
    MVMIOFileData *data = (MVMIOFileData *)h->body.data;
    if (MVM_platform_fsync(data->fd) == -1)
        MVM_exception_throw_adhoc(tc, "Failed to flush filehandle: %s", strerror(errno));
}

/* Truncates the file handle. */
static void truncatefh(MVMThreadContext *tc, MVMOSHandle *h, MVMint64 bytes) {
    MVMIOFileData *data = (MVMIOFileData *)h->body.data;
    if (ftruncate(data->fd, bytes) == -1)
        MVM_exception_throw_adhoc(tc, "Failed to truncate filehandle: %s", strerror(errno));
}

/* Operations aiding process spawning and I/O handling. */
static void bind_stdio_handle(MVMThreadContext *tc, MVMOSHandle *h, uv_stdio_container_t *stdio) {
    MVMIOFileData *data = (MVMIOFileData *)h->body.data;
    stdio->flags        = UV_INHERIT_FD;
    stdio->data.fd      = data->fd;
}

/* Locks a file. */
static MVMint64 lock(MVMThreadContext *tc, MVMOSHandle *h, MVMint64 flag) {
    MVMIOFileData *data = (MVMIOFileData *)h->body.data;

#ifdef _WIN32

    const DWORD len = 0xffffffff;
    const HANDLE hf = (HANDLE)_get_osfhandle(data->fd);
    OVERLAPPED offset;

    if (hf == INVALID_HANDLE_VALUE) {
        MVM_exception_throw_adhoc(tc, "Failed to lock filehandle: bad file descriptor");
    }

    flag = ((flag & MVM_FILE_FLOCK_NONBLOCK) ? LOCKFILE_FAIL_IMMEDIATELY : 0)
          + ((flag & MVM_FILE_FLOCK_TYPEMASK) == MVM_FILE_FLOCK_SHARED
                                       ? 0 : LOCKFILE_EXCLUSIVE_LOCK);

    memset (&offset, 0, sizeof(offset));
    if (LockFileEx(hf, flag, 0, len, len, &offset)) {
        return 1;
    }

    MVM_exception_throw_adhoc(tc, "Failed to lock filehandle: %d", GetLastError());

    return 0;

#else

    struct flock l;
    ssize_t r;
    int fc;
    const int fd = data->fd;

    l.l_whence = SEEK_SET;
    l.l_start = 0;
    l.l_len = 0;

    if ((flag & MVM_FILE_FLOCK_TYPEMASK) == MVM_FILE_FLOCK_SHARED)
        l.l_type = F_RDLCK;
    else
        l.l_type = F_WRLCK;

    fc = (flag & MVM_FILE_FLOCK_NONBLOCK) ? F_SETLK : F_SETLKW;

    do {
        r = fcntl(fd, fc, &l);
    } while (r == -1 && errno == EINTR);

    if (r == -1) {
        MVM_exception_throw_adhoc(tc, "Failed to lock filehandle: %d", errno);
    }

    return 1;
#endif
}

/* Unlocks a file. */
static void unlock(MVMThreadContext *tc, MVMOSHandle *h) {
    MVMIOFileData *data = (MVMIOFileData *)h->body.data;

#ifdef _WIN32

    const DWORD len = 0xffffffff;
    const HANDLE hf = (HANDLE)_get_osfhandle(data->fd);
    OVERLAPPED offset;

    if (hf == INVALID_HANDLE_VALUE) {
        MVM_exception_throw_adhoc(tc, "Failed to seek in filehandle: bad file descriptor");
    }

    memset (&offset, 0, sizeof(offset));
    if (UnlockFileEx(hf, 0, len, len, &offset)) {
        return;
    }

    MVM_exception_throw_adhoc(tc, "Failed to unlock filehandle: %d", GetLastError());
#else

    struct flock l;
    ssize_t r;
    const int fd = data->fd;

    l.l_whence = SEEK_SET;
    l.l_start = 0;
    l.l_len = 0;
    l.l_type = F_UNLCK;

    do {
        r = fcntl(fd, F_SETLKW, &l);
    } while (r == -1 && errno == EINTR);

    if (r == -1) {
        MVM_exception_throw_adhoc(tc, "Failed to unlock filehandle: %d", errno);
    }
#endif
}

/* Frees data associated with the handle. */
static void gc_free(MVMThreadContext *tc, MVMObject *h, void *d) {
    MVMIOFileData *data = (MVMIOFileData *)d;
    if (data) {
        if (data->filename)
            MVM_free(data->filename);
        MVM_free(data);
    }
}

/* IO ops table, populated with functions. */
static const MVMIOClosable      closable      = { closefh };
static const MVMIOSyncReadable  sync_readable = { read_bytes, mvm_eof };
static const MVMIOSyncWritable  sync_writable = { write_bytes, flush, truncatefh };
static const MVMIOSeekable      seekable      = { seek, mvm_tell };
static const MVMIOPipeable      pipeable      = { bind_stdio_handle };
static const MVMIOLockable      lockable      = { lock, unlock };
static const MVMIOIntrospection introspection = { is_tty, mvm_fileno };

static const MVMIOOps op_table = {
    &closable,
    &sync_readable,
    &sync_writable,
    NULL,
    NULL,
    NULL,
    &seekable,
    NULL,
    &pipeable,
    &lockable,
    &introspection,
    NULL,
    gc_free
};

/* Builds POSIX flag from mode string. */
static int resolve_open_mode(int *flag, const char *cp) {
    switch (*cp++) {
        case 'r': *flag = O_RDONLY; break;
        case '-': *flag = O_WRONLY; break;
        case '+': *flag = O_RDWR;   break;

        /* alias for "-c" or "-ct" if by itself */
        case 'w':
        *flag = *cp ? O_WRONLY | O_CREAT : O_WRONLY | O_CREAT | O_TRUNC;
        break;

        default:
        return 0;
    }

    for (;;) switch (*cp++) {
        case 0:
        return 1;

        case 'a': *flag |= O_APPEND; break;
        case 'c': *flag |= O_CREAT;  break;
        case 't': *flag |= O_TRUNC;  break;
        case 'x': *flag |= O_EXCL;   break;

        default:
        return 0;
    }
}

/* Opens a file, returning a synchronous file handle. */
MVMObject * MVM_file_open_fh(MVMThreadContext *tc, MVMString *filename, MVMString *mode) {
    char * const fname = MVM_string_utf8_c8_encode_C_string(tc, filename);
    int fd;
    int flag;
    STAT statbuf;

    /* Resolve mode description to flags. */
    char * const fmode  = MVM_string_utf8_encode_C_string(tc, mode);
    if (!resolve_open_mode(&flag, fmode)) {
        char *waste[] = { fname, fmode, NULL };
        MVM_exception_throw_adhoc_free(tc, waste,
            "Invalid open mode for file %s: %s", fname, fmode);
    }
    MVM_free(fmode);

    /* Try to open the file. */
    if ((fd = open((const char *)fname, flag, DEFAULT_MODE)) == -1) {
        char *waste[] = { fname, NULL };
        const char *err = strerror(errno);
        MVM_exception_throw_adhoc_free(tc, waste, "Failed to open file %s: %s", fname, err);
    }

    /*  Check that we didn't open a directory by accident.
        If fstat fails, just move on: Most of the documented error cases should
        already have triggered when opening the file, and we can't do anything
        about the others; a failure also does not necessarily imply that the
        file descriptor cannot be used for reading/writing. */
    if (fstat(fd, &statbuf) == 0 && (statbuf.st_mode & S_IFMT) == S_IFDIR) {
        char *waste[] = { fname, NULL };
        if (close(fd) == -1) {
            const char *err = strerror(errno);
            MVM_exception_throw_adhoc_free(tc, waste,
                "Tried to open directory %s, which we failed to close: %s",
                fname, err);
        }
        MVM_exception_throw_adhoc_free(tc, waste, "Tried to open directory %s", fname);
    }

    /* Set up handle. */
    {
        MVMIOFileData * const data   = MVM_calloc(1, sizeof(MVMIOFileData));
        MVMOSHandle   * const result = (MVMOSHandle *)MVM_repr_alloc_init(tc,
            tc->instance->boot_types.BOOTIO);
        data->fd          = fd;
        data->filename    = fname;
        result->body.ops  = &op_table;
        result->body.data = data;
        return (MVMObject *)result;
    }
}

/* Opens a file, returning a synchronous file handle. */
MVMObject * MVM_file_handle_from_fd(MVMThreadContext *tc, int fd) {
    MVMOSHandle   * const result = (MVMOSHandle *)MVM_repr_alloc_init(tc, tc->instance->boot_types.BOOTIO);
    MVMIOFileData * const data   = MVM_calloc(1, sizeof(MVMIOFileData));
    data->fd          = fd;
    result->body.ops  = &op_table;
    result->body.data = data;
    return (MVMObject *)result;
}
