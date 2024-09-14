#if !defined(MVM_CAN_UNALIGNED_INT64) || !defined(MVM_CAN_UNALIGNED_NUM64)
#define MVM_ALIGN_SIZE(size) MVM_ALIGN_SECTION(size)
#else
#define MVM_ALIGN_SIZE(size) (size)
#endif
void * MVM_gc_allocate_nursery(struct MVMThreadContext *tc, size_t size);
MVMSTable * MVM_gc_allocate_stable(struct MVMThreadContext *tc, const MVMREPROps *repr, MVMObject *how);
MVMObject * MVM_gc_allocate_type_object(struct MVMThreadContext *tc, MVMSTable *st);
MVMObject * MVM_gc_allocate_object(struct MVMThreadContext *tc, MVMSTable *st);
MVMFrame * MVM_gc_allocate_frame(struct MVMThreadContext *tc);

static inline void * MVM_gc_allocate(struct MVMThreadContext *tc, size_t size) {
    return tc->allocate_in_gen2
        ? MVM_gc_gen2_allocate_zeroed(tc->gen2, size)
        : MVM_gc_allocate_nursery(tc, size);
}

/* Same as MVM_gc_allocate, but promises that the memory will be zeroed. */
static inline void * MVM_gc_allocate_zeroed(struct MVMThreadContext *tc, size_t size) {
    /* At present, MVM_gc_allocate always returns zeroed memory. */
    return MVM_gc_allocate(tc, size);
}

/* Sets allocate for this thread to be from the second generation by
 * default. */
static inline void MVM_gc_allocate_gen2_default_set(struct MVMThreadContext *tc) {
    tc->allocate_in_gen2++;
}

/* Sets allocation for this thread to be from the nursery by default. */
static inline void MVM_gc_allocate_gen2_default_clear(struct MVMThreadContext *tc) {
    if (tc->allocate_in_gen2 <= 0)
        MVM_oops(tc, "Cannot leave gen2 allocation without entering it");
    tc->allocate_in_gen2--;
}
