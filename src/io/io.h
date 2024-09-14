/* Operation table for I/O. A given handle type may implement any number of
 * these sections. */
struct MVMIOOps {
    /* The various sections that may be implemented. */
    const MVMIOClosable        *closable;
    const MVMIOSyncReadable    *sync_readable;
    const MVMIOSyncWritable    *sync_writable;
    const MVMIOAsyncReadable   *async_readable;
    const MVMIOAsyncWritable   *async_writable;
    const MVMIOAsyncWritableTo *async_writable_to;
    const MVMIOSeekable        *seekable;
    const MVMIOSockety         *sockety;
    MVMObject * (*get_async_task_handle) (MVMThreadContext *tc, MVMOSHandle *h);
    const MVMIOLockable        *lockable;
    const MVMIOIntrospection   *introspection;
    void (*set_buffer_size) (MVMThreadContext *tc, MVMOSHandle *h, int64_t size);

    /* How to mark the handle's data, if needed. */
    void (*gc_mark) (MVMThreadContext *tc, void *data, MVMGCWorklist *worklist);

    /* How to free the handle's data. */
    void (*gc_free) (MVMThreadContext *tc, MVMObject *h, void *data);
};

/* I/O operations on handles that can be closed. */
struct MVMIOClosable {
    int64_t (*close) (MVMThreadContext *tc, MVMOSHandle *h);
};

/* I/O operations on handles that can do synchronous reading. */
struct MVMIOSyncReadable {
    int64_t (*read_bytes) (MVMThreadContext *tc, MVMOSHandle *h, char **buf, uint64_t bytes);
    int64_t (*eof) (MVMThreadContext *tc, MVMOSHandle *h);
};

/* I/O operations on handles that can do synchronous writing. */
struct MVMIOSyncWritable {
    int64_t (*write_bytes) (MVMThreadContext *tc, MVMOSHandle *h, char *buf, uint64_t bytes);
    void (*flush) (MVMThreadContext *tc, MVMOSHandle *h, int32_t sync);
    void (*truncate) (MVMThreadContext *tc, MVMOSHandle *h, int64_t bytes);
};

/* I/O operations on handles that can do asynchronous reading. */
struct MVMIOAsyncReadable {
    MVMAsyncTask * (*read_bytes) (MVMThreadContext *tc, MVMOSHandle *h, MVMObject *queue,
        MVMObject *schedulee, MVMObject *buf_type, MVMObject *async_type);
};

/* I/O operations on handles that can do asynchronous writing. */
struct MVMIOAsyncWritable {
    MVMAsyncTask * (*write_bytes) (MVMThreadContext *tc, MVMOSHandle *h, MVMObject *queue,
        MVMObject *schedulee, MVMObject *buffer, MVMObject *async_type);
};

/* I/O operations on handles that can do asynchronous writing to a given
 * network destination. */
struct MVMIOAsyncWritableTo {
    MVMAsyncTask * (*write_bytes_to) (MVMThreadContext *tc, MVMOSHandle *h, MVMObject *queue,
        MVMObject *schedulee, MVMObject *buffer, MVMObject *async_type, MVMString *host, int64_t port);
};

/* I/O operations on handles that can seek/tell. */
struct MVMIOSeekable {
    void (*seek) (MVMThreadContext *tc, MVMOSHandle *h, int64_t offset, int64_t whence);
    int64_t (*tell) (MVMThreadContext *tc, MVMOSHandle *h);
};

/* I/O operations on handles that do socket-y things (connect, bind, accept). */
struct MVMIOSockety {
    void (*connect) (MVMThreadContext *tc, MVMOSHandle *h, MVMString *host, int64_t port, uint16_t family);
    void (*bind) (MVMThreadContext *tc, MVMOSHandle *h, MVMString *host, int64_t port, uint16_t family, int32_t backlog);
    MVMObject * (*accept) (MVMThreadContext *tc, MVMOSHandle *h);
    int64_t (*getport) (MVMThreadContext *tc, MVMOSHandle *h);
};

/* I/O operations on handles that can lock/unlock. */
struct MVMIOLockable {
    int64_t (*lock) (MVMThreadContext *tc, MVMOSHandle *h, int64_t flag);
    void (*unlock) (MVMThreadContext *tc, MVMOSHandle *h);
};

/* Various bits of introspection we can perform on a handle. */
struct MVMIOIntrospection {
    int64_t (*is_tty) (MVMThreadContext *tc, MVMOSHandle *h);
    int64_t (*native_descriptor) (MVMThreadContext *tc, MVMOSHandle *h);
    int64_t (*mvm_open_mode) (MVMThreadContext *tc, MVMOSHandle *h);
};

int64_t MVM_io_close(MVMThreadContext *tc, MVMObject *oshandle);
int64_t MVM_io_is_tty(MVMThreadContext *tc, MVMObject *oshandle);
int64_t MVM_io_fileno(MVMThreadContext *tc, MVMObject *oshandle);
void MVM_io_seek(MVMThreadContext *tc, MVMObject *oshandle, int64_t offset, int64_t flag);
int64_t MVM_io_tell(MVMThreadContext *tc, MVMObject *oshandle);
void MVM_io_read_bytes(MVMThreadContext *tc, MVMObject *oshandle, MVMObject *result, int64_t length);
void MVM_io_write_bytes(MVMThreadContext *tc, MVMObject *oshandle, MVMObject *buffer);
void MVM_io_write_bytes_c(MVMThreadContext *tc, MVMObject *oshandle, char *output,
    uint64_t output_size);
MVMObject * MVM_io_read_bytes_async(MVMThreadContext *tc, MVMObject *oshandle, MVMObject *queue,
    MVMObject *schedulee, MVMObject *buf_type, MVMObject *async_type);
MVMObject * MVM_io_write_bytes_async(MVMThreadContext *tc, MVMObject *oshandle, MVMObject *queue,
        MVMObject *schedulee, MVMObject *buffer, MVMObject *async_type);
MVMObject * MVM_io_write_bytes_to_async(MVMThreadContext *tc, MVMObject *oshandle, MVMObject *queue,
        MVMObject *schedulee, MVMObject *buffer, MVMObject *async_type, MVMString *host, int64_t port);
int64_t MVM_io_eof(MVMThreadContext *tc, MVMObject *oshandle);
int64_t MVM_io_lock(MVMThreadContext *tc, MVMObject *oshandle, int64_t flag);
void MVM_io_unlock(MVMThreadContext *tc, MVMObject *oshandle);
void MVM_io_flush(MVMThreadContext *tc, MVMObject *oshandle, int32_t sync);
void MVM_io_truncate(MVMThreadContext *tc, MVMObject *oshandle, int64_t offset);
void MVM_io_connect(MVMThreadContext *tc, MVMObject *oshandle, MVMString *host, int64_t port, uint16_t family);
void MVM_io_bind(MVMThreadContext *tc, MVMObject *oshandle, MVMString *host, int64_t port, uint16_t family, int32_t backlog);
MVMObject * MVM_io_accept(MVMThreadContext *tc, MVMObject *oshandle);
int64_t MVM_io_getport(MVMThreadContext *tc, MVMObject *oshandle);
void MVM_io_set_buffer_size(MVMThreadContext *tc, MVMObject *oshandle, int64_t size);
MVMObject * MVM_io_get_async_task_handle(MVMThreadContext *tc, MVMObject *oshandle);
void MVM_io_flush_standard_handles(MVMThreadContext *tc);

#ifdef _WIN32
int MVM_set_std_handle_to_nul(FILE* file, int fd, BOOL read, int std_handle_type);
#endif
