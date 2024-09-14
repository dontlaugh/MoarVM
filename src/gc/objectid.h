uint64_t MVM_gc_object_id(struct MVMThreadContext *tc, MVMObject *obj);
void * MVM_gc_object_id_use_allocation(struct MVMThreadContext *tc, MVMCollectable *item);
void MVM_gc_object_id_clear(struct MVMThreadContext *tc, MVMCollectable *item);
