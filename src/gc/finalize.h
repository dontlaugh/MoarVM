void MVM_gc_finalize_set(MVMThreadContext *tc, MVMObject *type, int64_t finalize);
void MVM_gc_finalize_add_to_queue(MVMThreadContext *tc, MVMObject *obj);
void MVM_finalize_walk_queues(MVMThreadContext *tc, uint8_t gen);
void MVM_gc_finalize_run_handler(MVMThreadContext *tc);
