/* The result produced from specializing bytecode. */
struct MVMSpeshCode {
    /* The specialized bytecode. */
    uint8_t *bytecode;

    /* The size of the produced bytecode. */
    uint32_t bytecode_size;

    /* Updated set of frame handlers. */
    MVMFrameHandler *handlers;

    /* Deopt usage info, which will be stored on the candidate. */
    int32_t *deopt_usage_info;

    int32_t *deopt_synths;
    uint64_t num_deopt_synths;
};

MVMSpeshCode * MVM_spesh_codegen(MVMThreadContext *tc, MVMSpeshGraph *g);
