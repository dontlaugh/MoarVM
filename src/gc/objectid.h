#ifndef OBJECTID_H
#define OBJECTID_H

MVMuint64 MVM_gc_object_id(MVMThreadContext *tc, MVMObject *obj);
void * MVM_gc_object_id_use_allocation(MVMThreadContext *tc, MVMCollectable *item);
void MVM_gc_object_id_clear(MVMThreadContext *tc, MVMCollectable *item);
#endif // OBJECTID_H
