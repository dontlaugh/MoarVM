

/* Set this flag to debug temporary root pushes/pops. */
#define MVM_TEMP_ROOT_DEBUG 0

/* The number of temp roots we start out with per thread (and so can rely on
 * always having). */
#define MVM_TEMP_ROOT_BASE_ALLOC 16

/* Temp root push slow-path case. */
 void MVM_gc_root_temp_push_slow(struct MVMThreadContext *tc, MVMCollectable **obj_ref);

/* Fast-path case of pushing a root onto the per-thread temporary roots. */
static inline void MVM_gc_root_temp_push(struct MVMThreadContext *tc, MVMCollectable **obj_ref) {
    /* If debugging, ensure the root is not null. */
#if MVM_TEMP_ROOT_DEBUG
    if (obj_ref == NULL)
        MVM_panic(MVM_exitcode_gcroots, "Illegal attempt to add null object address as a temporary root");
#endif

    /* If less than the number of always-allocated roots, just add. */
    if (MVM_LIKELY(tc->num_temproots < MVM_TEMP_ROOT_BASE_ALLOC)) {
        tc->temproots[tc->num_temproots] = obj_ref;
        tc->num_temproots++;
    }

    /* Otherwise call the slow path. */
    else {
        MVM_gc_root_temp_push_slow(tc, obj_ref);
    }
}

/* Special forms of root pushing only needed for the MVMROOT macros */
static uint8_t __MVM_gc_root_temp_push_ensure_space_slow(struct MVMThreadContext *tc, uint8_t amount) {
    if (tc->num_temproots + amount > tc->alloc_temproots) {
        tc->alloc_temproots *= 2;
        tc->temproots = MVM_realloc(tc->temproots,
            sizeof(MVMCollectable **) * tc->alloc_temproots);
    }
    return 1;
}
static inline uint8_t __MVM_gc_root_temp_push_ensure_space(struct MVMThreadContext *tc, uint8_t amount) {
    /* If less than the number of always-allocated roots, we are happy */
    if (MVM_LIKELY(tc->num_temproots + amount < MVM_TEMP_ROOT_BASE_ALLOC)) {
        return 1;
    }
    else {
        /* Make a call into the slow path, we don't ask for this one
         * to be inlined. */
        __MVM_gc_root_temp_push_ensure_space_slow(tc, amount);
        return 1;
    }
}
/* We only use this after having manually assured that there is space. */
static inline uint8_t __MVM_gc_root_temp_push_nonvoid_noslow(struct MVMThreadContext *tc, MVMCollectable **obj_ref, uint8_t chain_in) {
    /* If debugging, ensure the root is not null. */
#if MVM_TEMP_ROOT_DEBUG
    if (obj_ref == NULL)
        MVM_panic(MVM_exitcode_gcroots, "Illegal attempt to add null object address as a temporary root");
#endif

    tc->temproots[tc->num_temproots] = obj_ref;
    tc->num_temproots++;
    return 1;
}
/* Special version of gc_root_temp_push that returns a value so it is
 * allowed to be chained together without the compiler complaining. */
static inline uint8_t __MVM_gc_root_temp_push_nonvoid(struct MVMThreadContext *tc,struct  MVMCollectable **obj_ref, uint8_t chain_in) {
    MVM_gc_root_temp_push(tc, obj_ref);
    return 1;
}

/* Pop top root from the per-thread temporary roots stack. */
static inline void MVM_gc_root_temp_pop(struct MVMThreadContext *tc) {
#if MVM_TEMP_ROOT_DEBUG
    if (tc->num_temproots <= 0)
        MVM_panic(1, "Illegal attempt to pop empty temporary root stack");
#endif
    tc->num_temproots--;
}

/* Pop top n roots from the per-thread temporary roots stack. */
static inline void MVM_gc_root_temp_pop_n(struct MVMThreadContext *tc, uint32_t n) {
#if MVM_TEMP_ROOT_DEBUG
    if (tc->num_temproots < n)
        MVM_panic(MVM_exitcode_gcroots, "Illegal attempt to pop insufficiently large temporary root stack");
#endif
    tc->num_temproots -= n;
}

/* Other functions related to roots. */
 void MVM_gc_root_add_permanent(struct MVMThreadContext *tc, MVMCollectable **obj_ref);
 void MVM_gc_root_add_permanent_desc(struct MVMThreadContext *tc, MVMCollectable **obj_ref, const char *description);
void MVM_gc_root_add_permanents_to_worklist(struct MVMThreadContext *tc, MVMGCWorklist *worklist, MVMHeapSnapshotState *snapshot);
void MVM_gc_root_add_instance_roots_to_worklist(struct MVMThreadContext *tc, MVMGCWorklist *worklist, MVMHeapSnapshotState *snapshot);
void MVM_gc_root_add_tc_roots_to_worklist(struct MVMThreadContext *tc, MVMGCWorklist *worklist, MVMHeapSnapshotState *snapshot);
uint32_t MVM_gc_root_temp_mark(struct MVMThreadContext *tc);
void MVM_gc_root_temp_mark_reset(struct MVMThreadContext *tc, uint32_t mark);
void MVM_gc_root_temp_pop_all(struct MVMThreadContext *tc);
void MVM_gc_root_add_temps_to_worklist(struct MVMThreadContext *tc, MVMGCWorklist *worklist, MVMHeapSnapshotState *snapshot);
void MVM_gc_root_gen2_add(struct MVMThreadContext *tc, MVMCollectable *c);
void MVM_gc_root_add_gen2s_to_worklist(struct MVMThreadContext *tc, MVMGCWorklist *worklist);
void MVM_gc_root_add_gen2s_to_snapshot(struct MVMThreadContext *tc, MVMHeapSnapshotState *snapshot);
void MVM_gc_root_gen2_cleanup(struct MVMThreadContext *tc);
void MVM_gc_root_add_frame_roots_to_worklist(struct MVMThreadContext *tc, MVMGCWorklist *worklist, MVMFrame *start_frame);
void MVM_gc_root_add_frame_registers_to_worklist(struct MVMThreadContext *tc, MVMGCWorklist *worklist, MVMFrame *frame);

/* C preprocessor macros are basically the worst thing ever.
 * So here's an explanation of the cool new MVMROOT macro:
 *
 * We want the whole macro to expand to a "for ( ...; ...; ...)"
 * line, so that
 * 1. we can put a block afterwards as if our root macro were control flow
 * 2. we can put code in the initialization part of the for loop body that
 *    pushes the temp roots
 * 3. we can put code in the step part of the for loop to pop the roots again
 * 4. we can put code in the check part that makes sure the block only
 *    runs the first time around.
 *
 * This is achieved by chaining __MVM_gc_root_temp_push_nonvoid calls into a
 * single expression that we use to initialize the "loop counter" variable to
 * 1, which at the end of the "loop" is set to 0.
 */

/* The specific rules of concatenation with ## force us to put one macro in
 * between what we want to concat and the ## operator.
 */
#define __MVM__CONCAT_IMPL( x, y ) x##y
#define __MVM__MACRO_CONCAT( x, y ) __MVM__CONCAT_IMPL( x, y )

// The variable that we set to 0 after the for loop runs so it terminates
#define __MVMROOT_VAR_NAME __MVM__MACRO_CONCAT(__MVMROOT_runned_, __LINE__)

#define __MVMROOT_PUSH(tc, obj_ref, chain) __MVM_gc_root_temp_push_nonvoid_noslow(tc, (MVMCollectable **)&(obj_ref), chain)

#define MVMROOT(tc, obj_ref1)  /* If you get "passed 3 arguments, but takes just 2" error, if you want to keep compatibility with older moar versions, write explicit MVM_gc_root_temp_push and _pop calls, otherwise move the curly braces outside the MVMROOT parenthesis. */ \
    for (uint8_t __MVMROOT_VAR_NAME = __MVM_gc_root_temp_push_nonvoid(tc, (MVMCollectable **)&(obj_ref1), 0); \
        __MVMROOT_VAR_NAME != 0; \
        MVM_gc_root_temp_pop(tc), \
            __MVMROOT_VAR_NAME = 0)

#define MVMROOT2(tc, obj_ref1, obj_ref2)  /* If you get "passed 4 arguments, but takes just 3" error, if you want to keep compatibility with older moar versions, write explicit MVM_gc_root_temp_push and _pop calls, otherwise move the curly braces outside the MVMROOT parenthesis. */ \
    for (uint8_t __MVMROOT_VAR_NAME = \
            __MVMROOT_PUSH(tc, obj_ref1, \
            __MVMROOT_PUSH(tc, obj_ref2, \
            __MVM_gc_root_temp_push_ensure_space(tc, 2))); \
        __MVMROOT_VAR_NAME != 0 \
        ; \
    MVM_gc_root_temp_pop_n(tc, 2), __MVMROOT_VAR_NAME = 0)

#define MVMROOT3(tc, obj_ref1, obj_ref2, obj_ref3)  /* If you get "passed 5 arguments, but takes just 4" error, if you want to keep compatibility with older moar versions, write explicit MVM_gc_root_temp_push and _pop calls, otherwise move the curly braces outside the MVMROOT parenthesis. */ \
    for (uint8_t __MVMROOT_VAR_NAME = \
            __MVMROOT_PUSH(tc, obj_ref1, \
            __MVMROOT_PUSH(tc, obj_ref2, \
            __MVMROOT_PUSH(tc, obj_ref3, \
            __MVM_gc_root_temp_push_ensure_space(tc, 3)))); \
        __MVMROOT_VAR_NAME != 0 \
        ; \
    MVM_gc_root_temp_pop_n(tc, 3), __MVMROOT_VAR_NAME = 0)

#define MVMROOT4(tc, obj_ref1, obj_ref2, obj_ref3, obj_ref4)  /* If you get "passed 6 arguments, but takes just 5" error, if you want to keep compatibility with older moar versions, write explicit MVM_gc_root_temp_push and _pop calls, otherwise move the curly braces outside the MVMROOT parenthesis. */ \
    for (uint8_t __MVMROOT_VAR_NAME = \
            __MVMROOT_PUSH(tc, obj_ref1, \
            __MVMROOT_PUSH(tc, obj_ref2, \
            __MVMROOT_PUSH(tc, obj_ref3, \
            __MVMROOT_PUSH(tc, obj_ref4, \
            __MVM_gc_root_temp_push_ensure_space(tc, 4))))); \
        __MVMROOT_VAR_NAME != 0 \
        ; \
    MVM_gc_root_temp_pop_n(tc, 4), __MVMROOT_VAR_NAME = 0)

#define MVMROOT5(tc, obj_ref1, obj_ref2, obj_ref3, obj_ref4, obj_ref5)  /* If you get "passed 7 arguments, but takes just 6" error, if you want to keep compatibility with older moar versions, write explicit MVM_gc_root_temp_push and _pop calls, otherwise move the curly braces outside the MVMROOT parenthesis. */ \
    for (uint8_t __MVMROOT_VAR_NAME = \
            __MVMROOT_PUSH(tc, obj_ref1, \
            __MVMROOT_PUSH(tc, obj_ref2, \
            __MVMROOT_PUSH(tc, obj_ref3, \
            __MVMROOT_PUSH(tc, obj_ref4, \
            __MVMROOT_PUSH(tc, obj_ref5, \
            __MVM_gc_root_temp_push_ensure_space(tc, 5)))))); \
        __MVMROOT_VAR_NAME != 0 \
        ; \
    MVM_gc_root_temp_pop_n(tc, 5), __MVMROOT_VAR_NAME = 0)

#define MVMROOT6(tc, obj_ref1, obj_ref2, obj_ref3, obj_ref4, obj_ref5, obj_ref6)  /* If you get "passed 8 arguments, but takes just 7" error, if you want to keep compatibility with older moar versions, write explicit MVM_gc_root_temp_push and _pop calls, otherwise move the curly braces outside the MVMROOT parenthesis. */ \
    for (uint8_t __MVMROOT_VAR_NAME = \
            __MVMROOT_PUSH(tc, obj_ref1, \
            __MVMROOT_PUSH(tc, obj_ref2, \
            __MVMROOT_PUSH(tc, obj_ref3, \
            __MVMROOT_PUSH(tc, obj_ref4, \
            __MVMROOT_PUSH(tc, obj_ref5, \
            __MVMROOT_PUSH(tc, obj_ref6, \
            __MVM_gc_root_temp_push_ensure_space(tc, 6))))))); \
        __MVMROOT_VAR_NAME != 0 \
        ; \
    MVM_gc_root_temp_pop_n(tc, 6), __MVMROOT_VAR_NAME = 0)
