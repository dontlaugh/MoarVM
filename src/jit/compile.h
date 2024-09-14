struct MVMJitCode {
    void     (*func_ptr)(struct MVMThreadContext *tc, MVMCompUnit *cu, void * label);
    size_t     size;
    uint8_t  *bytecode;

    MVMStaticFrame *sf;

    uint16_t *local_types;
    uint32_t  num_locals;

    /* The basic idea here is that /all/ label names are indexes into the single
     * labels array. This isn't particularly efficient at runtime (because we
     * need a second dereference to figure the labels out), but very simple for
     * me now, and super-easy to optimise at a later date */
    uint32_t  num_labels;
    void     **labels;
    void      *exit_label;

    uint32_t      num_deopts;
    uint32_t      num_inlines;
    uint32_t      num_handlers;
    MVMJitDeopt    *deopts;
    MVMJitInline  *inlines;
    MVMJitHandler *handlers;

    uint32_t      spill_size;
    uint32_t      seq_nr;

    atomic_uintptr_t ref_cnt;
};

MVMJitCode* MVM_jit_compile_graph(struct MVMThreadContext *tc, MVMJitGraph *graph);

MVMJitCode* MVM_jit_code_copy(struct MVMThreadContext *tc, MVMJitCode * const code);
void MVM_jit_code_destroy(struct MVMThreadContext *tc, MVMJitCode *code);

/* Peseudotile compile functions */
void MVM_jit_compile_label(struct MVMThreadContext *tc, MVMJitCompiler *compiler,
                           MVMJitTile *tile, MVMJitExprTree *tree);
void MVM_jit_compile_branch(struct MVMThreadContext *tc, MVMJitCompiler *compiler,
                            MVMJitTile *tile, MVMJitExprTree *tree);
void MVM_jit_compile_conditional_branch(struct MVMThreadContext *tc, MVMJitCompiler *compiler,
                                        MVMJitTile *tile, MVMJitExprTree *tree);
void MVM_jit_compile_store(struct MVMThreadContext *tc, MVMJitCompiler *compiler,
                           MVMJitTile *tile, MVMJitExprTree *tree);
void MVM_jit_compile_load(struct MVMThreadContext *tc, MVMJitCompiler *compiler,
                          MVMJitTile *tile, MVMJitExprTree *tree);
void MVM_jit_compile_move(struct MVMThreadContext *tc, MVMJitCompiler *compiler,
                          MVMJitTile *tile, MVMJitExprTree *tree);
void MVM_jit_compile_memory_copy(struct MVMThreadContext *tc, MVMJitCompiler *compiler,
                                 MVMJitTile *tile, MVMJitExprTree *tree);
void MVM_jit_compile_guard(struct MVMThreadContext *tc, MVMJitCompiler *compiler,
                           MVMJitTile *tile, MVMJitExprTree *tree);
