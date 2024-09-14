/* The result produced from specializing bytecode. */
struct MVMSpeshCode {
    /* The specialized bytecode. */
    MVMuint8 *bytecode;

    /* The size of the produced bytecode. */
    MVMuint32 bytecode_size;

    /* Updated set of frame handlers. */
    MVMFrameHandler *handlers;

    /* Deopt usage info, which will be stored on the candidate. */
    int32_t *deopt_usage_info;

    int32_t *deopt_synths;
    MVMuint64 num_deopt_synths;
};

MVMSpeshCode * MVM_spesh_codegen(MVMThreadContext *tc, MVMSpeshGraph *g);
