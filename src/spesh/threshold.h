/* The maximum size of bytecode we'll ever attempt to optimize. */
#ifndef THRESHOLD_H
#define THRESHOLD_H
#define MVM_SPESH_MAX_BYTECODE_SIZE 65536

MVMuint32 MVM_spesh_threshold(MVMThreadContext *tc, MVMStaticFrame *sf);
#endif // THRESHOLD_H
