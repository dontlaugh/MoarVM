MVMObject * MVM_io_get_signals(MVMThreadContext *tc);
MVMObject * MVM_io_signal_handle(MVMThreadContext *tc,
    MVMObject *setup_notify_queue, MVMObject *setup_notify_schedulee,
    MVMObject *queue, MVMObject *schedulee, int64_t signal, MVMObject *async_type);
