void MVM_spesh_deopt_all(struct MVMThreadContext *tc);
void MVM_spesh_deopt_one(struct MVMThreadContext *tc, uint32_t deopt_idx);
int32_t MVM_spesh_deopt_find_inactive_frame_deopt_idx(struct MVMThreadContext *tc,
        MVMFrame *frame, MVMSpeshCandidate *spesh_cand);
void MVM_spesh_deopt_during_unwind(struct MVMThreadContext *tc);
static inline uint32_t MVM_spesh_deopt_bytecode_pos(uint32_t deopt) {
    return deopt >> 1;
}
static inline uint32_t MVM_spesh_deopt_is_pre(uint32_t deopt) {
    return deopt & 1;
}
