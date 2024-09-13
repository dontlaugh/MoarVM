#ifndef TIMERS_H
#define TIMERS_H

MVMObject * MVM_io_timer_create(MVMThreadContext *tc, MVMObject *queue,
                                MVMObject *schedulee, MVMint64 timeout, MVMint64 repeat, MVMObject *async_type);
#endif // TIMERS_H
