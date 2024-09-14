void MVM_profile_dump_instrumented_data(struct MVMThreadContext *tc);
void MVM_profile_instrument(struct MVMThreadContext *tc, MVMStaticFrame *sf);
void MVM_profile_ensure_uninstrumented(struct MVMThreadContext *tc, MVMStaticFrame *sf);
void MVM_profile_instrumented_start(struct MVMThreadContext *tc, MVMObject *config);
MVMObject * MVM_profile_instrumented_end(struct MVMThreadContext *tc);
void MVM_profile_free_node(struct MVMThreadContext *tc, MVMProfileCallNode *node);
void MVM_profile_instrumented_free_data(struct MVMThreadContext *tc);
void MVM_profile_instrumented_mark_data(struct MVMThreadContext *tc, MVMGCWorklist *worklist);
