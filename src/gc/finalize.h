#ifndef FINALIZE_H
#define FINALIZE_H

void MVM_gc_finalize_set(MVMThreadContext *tc, MVMObject *type, MVMint64 finalize);
void MVM_gc_finalize_add_to_queue(MVMThreadContext *tc, MVMObject *obj);
void MVM_finalize_walk_queues(MVMThreadContext *tc, MVMuint8 gen);
void MVM_gc_finalize_run_handler(MVMThreadContext *tc);
#endif // FINALIZE_H
