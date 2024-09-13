#ifndef ASYNCSOCKETUDP_H
#define ASYNCSOCKETUDP_H

MVMObject * MVM_io_socket_udp_async(MVMThreadContext *tc, MVMObject *queue,
                                    MVMObject *schedulee, MVMString *host,
                                    MVMint64 port, MVMint64 flags,
                                    MVMObject *async_type);
#endif // ASYNCSOCKETUDP_H
