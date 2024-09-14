/* Per-thread profiling data. */
struct MVMProfileThreadData {
    /* The current call graph node we're in. */
    MVMProfileCallNode *current_call;

    /* Cache whether moar currently has confprogs enabled */
    uint8_t is_dynamic_confprog_installed;
    uint8_t is_static_confprog_installed;

    /* The thread ID of the thread responsible for spawning this thread. */
    uint32_t parent_thread_id;

    /* The root of the call graph. */
    MVMProfileCallNode *call_graph;

    /* The time we started profiling. */
    uint64_t start_time;

    /* The time we finished profiling, if we got there already. */
    uint64_t end_time;

    /* The next two arrays are there to remove the need for the GC
     * to walk the entire call graph every time.
     */

    /* Replaces static frame pointers in call nodes with an index into
     * this array */
    MVM_VECTOR_DECL(MVMStaticFrame *, staticframe_array);

    /* Replaces type pointers in call nodes allocation entries with an
     * index into this array */
    MVM_VECTOR_DECL(MVMObject *, type_array);

    /* When a confprog is deciding what frames to enter, exiting a frame
     * when there's no node on the call stack can either be a run-time error,
     * or it can be completely normal. To differentiate, we have to count the
     * number of times we entered a frame without setting up a call graph node.
     */
    uint32_t non_calltree_depth;

    /* Garbage collection time measurements. */
    MVMProfileGC *gcs;
    uint32_t num_gcs;
    uint32_t alloc_gcs;

    /* Amount of time spent in spesh. */
    uint64_t spesh_time;

    /* Current spesh work start time, if any. */
    uint64_t cur_spesh_start_time;

    /* Current GC start time, if any. */
    uint64_t cur_gc_start_time;

    /* We would like to split "promoted bytes" into managed and
     * unmanaged, so that subtracting it from the nursery size
     * doesn't give us negative values. */
    uint64_t gc_promoted_unmanaged_bytes;

    /* We have to make sure to not count the newest allocation infinitely
     * often if there's a conditionally-allocating operation (like getlex)
     * that gets called multiple times with no actual allocations in between */
    MVMObject *last_counted_allocation;

    /* Used to pass generated data structure from the gc-orchestrated
     * dumping function back to the dump function that ends the profile */
    MVMObject *collected_data;
};

/* Information collected about a GC run. */
struct MVMProfileGC {
    /* How long the collection took. */
    uint64_t time;

    /* When, relative to program start, did this GC take place? */
    uint64_t abstime;

    /* Was it a full collection? */
    uint16_t full;

    /* Was this thread responsible? */
    uint16_t responsible;

    /* Which GC run does this belong to?
     * (Good to know in multithreaded situations where
     * some threads have their work stolen) */
    AO_t gc_seq_num;

    /* Nursery statistics. */
    uint32_t cleared_bytes;
    uint32_t retained_bytes;
    uint32_t promoted_bytes;

    uint64_t promoted_unmanaged_bytes;

    /* Inter-generation links count */
    uint32_t num_gen2roots;

    /* sum of num_gen2roots of all TCs that had work stolen by
     * this thread */
    uint32_t num_stolen_gen2roots;

    MVMProfileDeallocationCount *deallocs;
    uint32_t num_dealloc;
    uint32_t alloc_dealloc; /* haha */
};

/* Call graph node, which is kept per thread. */
struct MVMProfileCallNode {
    /* The frame this data is for.
     * If this CallNode is for a native call, this is NULL. */
    uint32_t sf_idx;
    /* The timestamp when we entered the node. */
    uint64_t cur_entry_time;

    /* Time we should skip since cur_entry_time because execution was
     * suspended due to GC or spesh. */
    uint64_t cur_skip_time;

    /* Entry mode, persisted for the sake of continuations. */
    uint64_t entry_mode;

    /* The node in the profiling call graph that we came from. */
    MVMProfileCallNode *pred;

    /* Successor nodes so far. */
    MVMProfileCallNode **succ;

    /* Number of successors we have, and have allocated space for. */
    uint32_t num_succ;
    uint32_t alloc_succ;

    /* Allocations of different types, and the number of allocation
     * counts we have so far. */
    MVMProfileAllocationCount *alloc;
    uint32_t num_alloc;
    uint32_t alloc_alloc;

    /* The total inclusive time so far spent in this node. */
    uint64_t total_time;

    /* The total number of times this node was entered. */
    uint64_t total_entries;

    /* Entries that were to specialized bytecode. */
    uint64_t specialized_entries;

    /* Entries that were inlined. */
    uint64_t inlined_entries;

    /* Entries that were to JITted code. */
    uint64_t jit_entries;

    /* Number of times OSR took place. */
    uint64_t osr_count;

    /* Number of times deopt_one happened. */
    uint64_t deopt_one_count;

    /* Number of times deopt_all happened. */
    uint64_t deopt_all_count;

    /* If the static frame is NULL, we're collecting data on a native call */
    char *native_target_name;

    /* When was this node first entered, ever? */
    uint64_t first_entry_time;

};

/* Allocation counts for a call node. */
struct MVMProfileAllocationCount {
    /* The type we're counting allocations of. */
    uint32_t type_idx;

    /* The number of allocations we've counted. */
    /* a) in regularly interpreted code */
    uint64_t allocations_interp;

    /* b) in spesh'd code */
    uint64_t allocations_spesh;

    /* c) in jitted code */
    uint64_t allocations_jit;

    /* The number of allocations elimianted thanks to scalar replacement. */
    uint64_t scalar_replaced;
};

struct MVMProfileDeallocationCount {
    MVMObject *type;

    /* How often was this type freed from the nursery with
     * the "seen in nursery" flag not set? */
    uint32_t deallocs_nursery_fresh;

    /* How often was this type freed from the nursery with
     * the "seen in nursery" flag set? */
    uint32_t deallocs_nursery_seen;

    /* How often was this type freed in the old generation? */
    uint32_t deallocs_gen2;
};

/* When a continuation is taken, we attach one of these to it. It carries the
 * data needed to restore profiler state if the continuation is invoked. */
struct MVMProfileContinuationData {
    /* List of static frames we should restore, in reverse order. */
    MVMStaticFrame **sfs;

    /* Entry modes to restore also. */
    uint64_t *modes;

    /* Number of static frames in the list. */
    uint64_t num_sfs;
};

/* Ways we might enter a frame. */
#define MVM_PROFILE_ENTER_NORMAL        0
#define MVM_PROFILE_ENTER_SPESH         1
#define MVM_PROFILE_ENTER_SPESH_INLINE  2
#define MVM_PROFILE_ENTER_JIT           3
#define MVM_PROFILE_ENTER_JIT_INLINE    4

/* Logging functions. */
void MVM_profile_log_enter(MVMThreadContext *tc, MVMStaticFrame *sf, uint64_t mode);
void MVM_profile_log_enter_native(MVMThreadContext *tc, MVMObject *nativecallsite);
void MVM_profile_log_exit(MVMThreadContext *tc);
void MVM_profile_log_unwind(MVMThreadContext *tc);
MVMProfileContinuationData * MVM_profile_log_continuation_control(MVMThreadContext *tc, const MVMFrame *root_frame);
void MVM_profile_log_continuation_invoke(MVMThreadContext *tc, const MVMProfileContinuationData *cd);
void MVM_profile_log_thread_created(MVMThreadContext *tc, MVMThreadContext *child_tc);
void MVM_profile_log_allocated(MVMThreadContext *tc, MVMObject *obj);
void MVM_profile_log_scalar_replaced(MVMThreadContext *tc, MVMSTable *st);
void MVM_profiler_log_gc_start(MVMThreadContext *tc, uint32_t full, uint32_t this_thread_responsible);
void MVM_profiler_log_gc_end(MVMThreadContext *tc);
void MVM_profiler_log_gen2_roots(MVMThreadContext *tc, uint64_t amount, MVMThreadContext *other);
void MVM_profiler_log_gc_deallocate(MVMThreadContext *tc, MVMObject *object);
void MVM_profiler_log_unmanaged_data_promoted(MVMThreadContext *tc, uint64_t amount);
void MVM_profiler_log_spesh_start(MVMThreadContext *tc);
void MVM_profiler_log_spesh_end(MVMThreadContext *tc);
void MVM_profiler_log_osr(MVMThreadContext *tc, uint64_t jitted);
void MVM_profiler_log_deopt_one(MVMThreadContext *tc);
void MVM_profiler_log_deopt_all(MVMThreadContext *tc);
