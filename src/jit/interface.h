/* Function for getting effective (JIT/specialized/original) bytecode. */
static inline uint8_t * MVM_frame_effective_bytecode(MVMFrame *f) {
    MVMSpeshCandidate *spesh_cand = f->spesh_cand;
    if (spesh_cand)
        return spesh_cand->body.jitcode ? spesh_cand->body.jitcode->bytecode : spesh_cand->body.bytecode;
    return f->static_info->body.bytecode;
}

/* Debugging assertion to check if JIT code is within a region. */
static inline void MVM_jit_code_assert_within_region(struct MVMThreadContext *tc, MVMJitCode *code,
        void *address) {
#if MVM_JIT_DEBUG
    int32_t ofs = (char*)address - (char*)code->func_ptr;
    if ((0 <= ofs) && (ofs < code->size))
        return;
    MVM_panic(1, "JIT: address out of range for code!\n"
              "(label %p, func_ptr %p, code size %lui, offset %li, seq nr %i)",
              address, code->func_ptr, code->size, ofs, code->seq_nr);
#endif
}

/* Set the position in the currently executing frame. Done as a static inline
 * since in non-debug builds this is pretty much a single memory write, so the
 * call cost would dominate. */
static inline void MVM_jit_code_set_cur_frame_position(struct MVMThreadContext *tc, MVMJitCode *code,
        void *position) {
    MVM_jit_code_assert_within_region(tc, code, position);
    assert(tc->jit_return_address != NULL);
    /* this overwrites the address on the stack that MVM_frame_invoke_code will ret to! */
    *tc->jit_return_address = position;
}

void MVM_jit_code_enter(struct MVMThreadContext *tc, MVMJitCode *code, MVMCompUnit *cu);
void * MVM_jit_code_get_current_position(struct MVMThreadContext *tc, MVMJitCode *code, MVMFrame *frame);
void MVM_jit_code_set_current_position(struct MVMThreadContext *tc, MVMJitCode *code, MVMFrame *frame, void *position);
uint32_t MVM_jit_code_get_active_deopt_idx(struct MVMThreadContext *tc, MVMJitCode *code, MVMFrame *frame);
/* split iterators because we don't want to allocate on this path */
uint32_t MVM_jit_code_get_active_handlers(struct MVMThreadContext *tc, MVMJitCode *code, void *current_position, uint32_t i);
uint32_t MVM_jit_code_get_active_inlines(struct MVMThreadContext *tc, MVMJitCode *code, void *current_position, uint32_t i);

/* hackish interface */
void MVM_jit_code_trampoline(struct MVMThreadContext *tc);
