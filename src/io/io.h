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
    MVMObject * (*get_async_task_handle) (struct MVMThreadContext *tc, MVMOSHandle *h);
    const MVMIOLockable        *lockable;
    const MVMIOIntrospection   *introspection;
    void (*set_buffer_size) (struct MVMThreadContext *tc, MVMOSHandle *h, int64_t size);

    /* How to mark the handle's data, if needed. */
    void (*gc_mark) (struct MVMThreadContext *tc, void *data, MVMGCWorklist *worklist);

    /* How to free the handle's data. */
    void (*gc_free) (struct MVMThreadContext *tc, MVMObject *h, void *data);
};

/* I/O operations on handles that can be closed. */
struct MVMIOClosable {
    int64_t (*close) (struct MVMThreadContext *tc, MVMOSHandle *h);
};

/* I/O operations on handles that can do synchronous reading. */
struct MVMIOSyncReadable {
    int64_t (*read_bytes) (struct MVMThreadContext *tc, MVMOSHandle *h, char **buf, uint64_t bytes);
    int64_t (*eof) (struct MVMThreadContext *tc, MVMOSHandle *h);
};

/* I/O operations on handles that can do synchronous writing. */
struct MVMIOSyncWritable {
    int64_t (*write_bytes) (struct MVMThreadContext *tc, MVMOSHandle *h, char *buf, uint64_t bytes);
    void (*flush) (struct MVMThreadContext *tc, MVMOSHandle *h, int32_t sync);
    void (*truncate) (struct MVMThreadContext *tc, MVMOSHandle *h, int64_t bytes);
};

/* I/O operations on handles that can do asynchronous reading. */
struct MVMIOAsyncReadable {
    MVMAsyncTask * (*read_bytes) (struct MVMThreadContext *tc, MVMOSHandle *h, MVMObject *queue,
        MVMObject *schedulee, MVMObject *buf_type, MVMObject *async_type);
};

/* I/O operations on handles that can do asynchronous writing. */
struct MVMIOAsyncWritable {
    MVMAsyncTask * (*write_bytes) (struct MVMThreadContext *tc, MVMOSHandle *h, MVMObject *queue,
        MVMObject *schedulee, MVMObject *buffer, MVMObject *async_type);
};

/* I/O operations on handles that can do asynchronous writing to a given
 * network destination. */
struct MVMIOAsyncWritableTo {
    MVMAsyncTask * (*write_bytes_to) (struct MVMThreadContext *tc, MVMOSHandle *h, MVMObject *queue,
        MVMObject *schedulee, MVMObject *buffer, MVMObject *async_type, MVMString *host, int64_t port);
};

/* I/O operations on handles that can seek/tell. */
struct MVMIOSeekable {
    void (*seek) (struct MVMThreadContext *tc, MVMOSHandle *h, int64_t offset, int64_t whence);
    int64_t (*tell) (struct MVMThreadContext *tc, MVMOSHandle *h);
};

/* I/O operations on handles that do socket-y things (connect, bind, accept). */
struct MVMIOSockety {
    void (*connect) (struct MVMThreadContext *tc, MVMOSHandle *h, MVMString *host, int64_t port, uint16_t family);
    void (*bind) (struct MVMThreadContext *tc, MVMOSHandle *h, MVMString *host, int64_t port, uint16_t family, int32_t backlog);
    MVMObject * (*accept) (struct MVMThreadContext *tc, MVMOSHandle *h);
    int64_t (*getport) (struct MVMThreadContext *tc, MVMOSHandle *h);
};

/* I/O operations on handles that can lock/unlock. */
struct MVMIOLockable {
    int64_t (*lock) (struct MVMThreadContext *tc, MVMOSHandle *h, int64_t flag);
    void (*unlock) (struct MVMThreadContext *tc, MVMOSHandle *h);
};

/* Various bits of introspection we can perform on a handle. */
struct MVMIOIntrospection {
    int64_t (*is_tty) (struct MVMThreadContext *tc, MVMOSHandle *h);
    int64_t (*native_descriptor) (struct MVMThreadContext *tc, MVMOSHandle *h);
    int64_t (*mvm_open_mode) (struct MVMThreadContext *tc, MVMOSHandle *h);
};

int64_t MVM_io_close(struct MVMThreadContext *tc, MVMObject *oshandle);
int64_t MVM_io_is_tty(struct MVMThreadContext *tc, MVMObject *oshandle);
int64_t MVM_io_fileno(struct MVMThreadContext *tc, MVMObject *oshandle);
void MVM_io_seek(struct MVMThreadContext *tc, MVMObject *oshandle, int64_t offset, int64_t flag);
int64_t MVM_io_tell(struct MVMThreadContext *tc, MVMObject *oshandle);
void MVM_io_read_bytes(struct MVMThreadContext *tc, MVMObject *oshandle, MVMObject *result, int64_t length);
void MVM_io_write_bytes(struct MVMThreadContext *tc, MVMObject *oshandle, MVMObject *buffer);
void MVM_io_write_bytes_c(struct MVMThreadContext *tc, MVMObject *oshandle, char *output,
    uint64_t output_size);
MVMObject * MVM_io_read_bytes_async(struct MVMThreadContext *tc, MVMObject *oshandle, MVMObject *queue,
    MVMObject *schedulee, MVMObject *buf_type, MVMObject *async_type);
MVMObject * MVM_io_write_bytes_async(struct MVMThreadContext *tc, MVMObject *oshandle, MVMObject *queue,
        MVMObject *schedulee, MVMObject *buffer, MVMObject *async_type);
MVMObject * MVM_io_write_bytes_to_async(struct MVMThreadContext *tc, MVMObject *oshandle, MVMObject *queue,
        MVMObject *schedulee, MVMObject *buffer, MVMObject *async_type, MVMString *host, int64_t port);
int64_t MVM_io_eof(struct MVMThreadContext *tc, MVMObject *oshandle);
int64_t MVM_io_lock(struct MVMThreadContext *tc, MVMObject *oshandle, int64_t flag);
void MVM_io_unlock(struct MVMThreadContext *tc, MVMObject *oshandle);
void MVM_io_flush(struct MVMThreadContext *tc, MVMObject *oshandle, int32_t sync);
void MVM_io_truncate(struct MVMThreadContext *tc, MVMObject *oshandle, int64_t offset);
void MVM_io_connect(struct MVMThreadContext *tc, MVMObject *oshandle, MVMString *host, int64_t port, uint16_t family);
void MVM_io_bind(struct MVMThreadContext *tc, MVMObject *oshandle, MVMString *host, int64_t port, uint16_t family, int32_t backlog);
MVMObject * MVM_io_accept(struct MVMThreadContext *tc, MVMObject *oshandle);
int64_t MVM_io_getport(struct MVMThreadContext *tc, MVMObject *oshandle);
void MVM_io_set_buffer_size(struct MVMThreadContext *tc, MVMObject *oshandle, int64_t size);
MVMObject * MVM_io_get_async_task_handle(struct MVMThreadContext *tc, MVMObject *oshandle);
void MVM_io_flush_standard_handles(struct MVMThreadContext *tc);

#ifdef _WIN32
int MVM_set_std_handle_to_nul(FILE* file, int fd, BOOL read, int std_handle_type);
#endif
