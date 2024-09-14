void MVM_jit_dump_bytecode(struct MVMThreadContext *tc, MVMJitCode *code);
void MVM_jit_dump_expr_tree(struct MVMThreadContext *tc, MVMJitExprTree *tree);
void MVM_jit_dump_tile_list(struct MVMThreadContext *tc, MVMJitTileList *list);

static inline int32_t MVM_jit_debug_enabled(struct MVMThreadContext *tc) {
    return MVM_spesh_debug_enabled(tc) && tc->instance->jit_debug_enabled;
}

static inline int32_t MVM_jit_bytecode_dump_enabled(struct MVMThreadContext *tc) {
    return tc->instance->jit_bytecode_dir != NULL;
}
