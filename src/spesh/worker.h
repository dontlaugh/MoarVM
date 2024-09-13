#ifndef WORKER_H
#define WORKER_H

void MVM_spesh_worker_start(MVMThreadContext *tc);
void MVM_spesh_worker_stop(MVMThreadContext *tc);
void MVM_spesh_worker_join(MVMThreadContext *tc);

#endif // WORKER_H
