MVMObject * MVM_io_socket_connect_async(struct MVMThreadContext *tc, MVMObject *queue,
    MVMObject *schedulee, MVMString *host, int64_t port, MVMObject *async_type);
MVMObject * MVM_io_socket_listen_async(struct MVMThreadContext *tc, MVMObject *queue,
    MVMObject *schedulee, MVMString *host, int64_t port, int32_t backlog, MVMObject *async_type);

MVMObject * MVM_io_socket_connect_unix_async(struct MVMThreadContext *tc, MVMObject *queue,
    MVMObject *schedulee, MVMString *path, MVMObject *async_type);
MVMObject * MVM_io_socket_listen_unix_async(struct MVMThreadContext *tc, MVMObject *queue,
    MVMObject *schedulee, MVMString *path, int32_t backlog, MVMObject *async_type);
