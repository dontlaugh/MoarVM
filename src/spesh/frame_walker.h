/* State maintained during a spesh-aware frame walk. */
struct MVMSpeshFrameWalker {
    /* The current real MVMFrame that we are considering. */
    MVMFrame *cur_caller_frame;

    /* If we're currently looking through applicable inlines, the JIT position
     * or deopt offset to look for them based on, together with the current inline
     * we are looking at. */
    void *jit_position;
    uint32_t deopt_offset;
    int32_t inline_idx;

    /* If we're doing a walk of outer frames too, the current outer frame that
     * we are considering starting from the caller frame. */
    MVMFrame *cur_outer_frame;

    /* Should we walk the outer chain hanging off each dynamic frame? */
    uint8_t visit_outers;

    /* Should we visit the callers beyond the start frame (maybe after some
     * traversals before we start to iterate)? */
    uint8_t visit_callers;

    /* Did we start iterating yet? */
    uint8_t started;

    /* Did we just do a manual traversal, and so are on the frame we want to
     * start off with? */
    uint8_t traversed;

    /* Are we currently visiting the outer chain? */
    uint8_t visiting_outers;
};

/* Sentinel value to indicate there's no inline to explore. */
#define MVM_SPESH_FRAME_WALKER_NO_INLINE -2

void MVM_spesh_frame_walker_init(struct MVMThreadContext *tc, MVMSpeshFrameWalker *fw, MVMFrame *start,
        uint8_t visit_outers);
void MVM_spesh_frame_walker_init_for_outers(struct MVMThreadContext *tc, MVMSpeshFrameWalker *fw,
    MVMFrame *start);
uint32_t MVM_spesh_frame_walker_next(struct MVMThreadContext *tc, MVMSpeshFrameWalker *fw);
uint32_t MVM_spesh_frame_walker_get_lex(struct MVMThreadContext *tc, MVMSpeshFrameWalker *fw,
        MVMString *name, MVMRegister **found_out, uint16_t *found_kind_out,
        uint32_t vivify, MVMFrame **found_frame);
uint32_t MVM_spesh_frame_walker_move_outer(struct MVMThreadContext *tc, MVMSpeshFrameWalker *fw);
uint32_t MVM_spesh_frame_walker_move_caller(struct MVMThreadContext *tc, MVMSpeshFrameWalker *fw);
uint32_t MVM_spesh_frame_walker_move_outer_skip_thunks(struct MVMThreadContext *tc,
        MVMSpeshFrameWalker *fw);
uint32_t MVM_spesh_frame_walker_move_caller_skip_thunks(struct MVMThreadContext *tc,
        MVMSpeshFrameWalker *fw);
MVMFrame * MVM_spesh_frame_walker_get_frame(struct MVMThreadContext *tc, MVMSpeshFrameWalker *fw);
MVMObject * MVM_spesh_frame_walker_get_lexicals_hash(struct MVMThreadContext *tc, MVMSpeshFrameWalker *fw);
int64_t MVM_spesh_frame_walker_get_lexical_primspec(struct MVMThreadContext *tc,
        MVMSpeshFrameWalker *fw, MVMString *name);
MVMObject * MVM_spesh_frame_walker_get_code(struct MVMThreadContext *tc, MVMSpeshFrameWalker *fw);
uint64_t MVM_spesh_frame_walker_get_lexical_count(struct MVMThreadContext *tc, MVMSpeshFrameWalker *fw);

/* Gets the current frame we're walking. If we're in an inline, then it's the
 * frame holding the inline. */
static inline MVMFrame * MVM_spesh_frame_walker_current_frame(struct MVMThreadContext *tc,
        MVMSpeshFrameWalker *fw) {
    return MVM_UNLIKELY(fw->visiting_outers)
        ? fw->cur_outer_frame
        : fw->cur_caller_frame;
}

/* Returns non-zero if we're currently visiting an inline, zero otherwise. */
static inline uint32_t MVM_spesh_frame_walker_is_inline(struct MVMThreadContext *tc,
        MVMSpeshFrameWalker *fw) {
    return fw->inline_idx != MVM_SPESH_FRAME_WALKER_NO_INLINE;
}

/* Cleans up the spesh frame walker after use. */
static inline void MVM_spesh_frame_walker_cleanup(struct MVMThreadContext *tc,
        MVMSpeshFrameWalker *fw) {
    MVM_gc_root_temp_pop_n(tc, 2);
}
