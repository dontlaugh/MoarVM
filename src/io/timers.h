MVMObject * MVM_io_timer_create(struct MVMThreadContext *tc, MVMObject *queue,
    MVMObject *schedulee, int64_t timeout, int64_t repeat, MVMObject *async_type);
