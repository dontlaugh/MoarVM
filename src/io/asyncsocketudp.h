MVMObject * MVM_io_socket_udp_async(struct MVMThreadContext *tc, MVMObject *queue,
                                    MVMObject *schedulee, MVMString *host,
                                    int64_t port, int64_t flags,
                                    MVMObject *async_type);
