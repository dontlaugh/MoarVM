struct MVMHeapDumpIndexSnapshotEntry {
    uint64_t collectables_size;
    uint64_t full_refs_size;
    uint64_t refs_middlepoint;
    uint64_t incremental_data;
};

/* Used in version 2 of the heap snapshot format */
struct MVMHeapDumpIndex {
    uint64_t stringheap_size;
    uint64_t types_size;
    uint64_t staticframes_size;

    uint64_t snapshot_size_entries;
    MVMHeapDumpIndexSnapshotEntry *snapshot_sizes;

    uint64_t snapshot_sizes_alloced;
};

/* Used in version 3 of the heap snapshot format.
 * There is one top-level TOC for the whole file,
 * but every snapshot in the file has a TOC of its
 * own that's got an entry in the top-level TOC.
 */
struct MVMHeapDumpTableOfContents {
    uint32_t toc_entry_alloc;
    uint32_t toc_entry_used;

    char **toc_words;
    uint64_t *toc_positions;
};

struct MVMHeapSnapshotStats {
    uint64_t type_stats_alloc;

    uint32_t *type_counts;
    uint64_t *type_size_sum;

    uint64_t sf_stats_alloc;

    uint32_t *sf_counts;
    uint64_t *sf_size_sum;
};

/* A collection of heap snapshots, with common type and static frame names.
 * Note that we take care to never refer to heap objects themselves in here,
 * including for types and frames, since to do so would extend their lifetime
 * for the whole program, which would render the results pretty bogus. */
struct MVMHeapSnapshotCollection {
    /* Snapshot we are currently taking and its index */
    MVMHeapSnapshot *snapshot;
    uint64_t snapshot_idx;

    /* Known types/REPRs. Just a list for now, but we might like to look at a
     * hash or trie if this ends up making taking a snapshot wicked slow. */
    MVMHeapSnapshotType *types;
    uint64_t num_types;
    uint64_t alloc_types;

    /* Known static frames. Same applies to searching this as to the above. */
    MVMHeapSnapshotStaticFrame *static_frames;
    uint64_t num_static_frames;
    uint64_t alloc_static_frames;

    /* Strings, referenced by index from various places. Also a "should we
     * free it" flag for each one. */
    char **strings;
    uint64_t num_strings;
    uint64_t alloc_strings;
    char *strings_free;
    uint64_t num_strings_free;
    uint64_t alloc_strings_free;

    uint64_t types_written;
    uint64_t static_frames_written;
    uint64_t strings_written;

    /* For heap snapshot format 2, an index */
    MVMHeapDumpIndex *index;

    /* For heap snapshot format 3, table of contents */
    MVMHeapDumpTableOfContents *toplevel_toc;
    MVMHeapDumpTableOfContents *second_level_toc;

    /* When the heap snapshot recording was started */
    uint64_t start_time;

    /* Counts for the current recording to make an overview */
    uint64_t total_heap_size;
    uint64_t total_objects;
    uint64_t total_typeobjects;
    uint64_t total_stables;
    uint64_t total_frames;

    /* The file handle we are outputting to */
    FILE *fh;
};

/* An individual heap snapshot. */
struct MVMHeapSnapshot {
    /* Array of data about collectables on the heap. */
    MVMHeapSnapshotCollectable *collectables;
    uint64_t num_collectables;
    uint64_t alloc_collectables;

    /* References.  */
    MVMHeapSnapshotReference *references;
    uint64_t num_references;
    uint64_t alloc_references;

    MVMHeapSnapshotStats *stats;

    /* When the snapshot was recorded */
    uint64_t record_time;
};

/* An object/type object/STable type in the snapshot. */
struct MVMHeapSnapshotType {
    /* String heap index of the REPR name. */
    uint32_t repr_name;

    /* String heap index of the type's debug name. */
    uint32_t type_name;
};

/* A static frame in the snapshot. */
struct MVMHeapSnapshotStaticFrame {
    /* The static frame name; index into the snapshot collection string heap. */
    uint32_t name;

    /* The static frame compilation unit ID, for added uniqueness checking.
     * Also an index into the string heap. */
    uint32_t cuid;

    /* The line number where it's declared. */
    uint32_t line;

    /* And the filename; also an index into snapshot collection string heap. */
    uint32_t file;
};

/* Kinds of collectable, plus a few "virtual" kinds to cover the various places
 * we find roots. MVM_SNAPSHOT_COL_KIND_ROOT is the ultimate root of the heap
 * snapshot and everything hangs off it. */
#define MVM_SNAPSHOT_COL_KIND_OBJECT            1
#define MVM_SNAPSHOT_COL_KIND_TYPE_OBJECT       2
#define MVM_SNAPSHOT_COL_KIND_STABLE            3
#define MVM_SNAPSHOT_COL_KIND_FRAME             4
#define MVM_SNAPSHOT_COL_KIND_PERM_ROOTS        5
#define MVM_SNAPSHOT_COL_KIND_INSTANCE_ROOTS    6
#define MVM_SNAPSHOT_COL_KIND_CSTACK_ROOTS      7
#define MVM_SNAPSHOT_COL_KIND_THREAD_ROOTS      8
#define MVM_SNAPSHOT_COL_KIND_ROOT              9
#define MVM_SNAPSHOT_COL_KIND_INTERGEN_ROOTS    10
#define MVM_SNAPSHOT_COL_KIND_CALLSTACK_ROOTS   11

/* Data about an individual collectable in the heap snapshot. Ordered to avoid
 * holes. */
struct MVMHeapSnapshotCollectable {
    /* What kind of collectable is it? */
    uint16_t kind;

    /* Self-size (from the collectable header). */
    uint16_t collectable_size;

    /* Index into the snapshot collection type name or frame info array,
     * depending on kind. */
    uint32_t type_or_frame_index;

    /* The number of other collectables this one references. */
    uint32_t num_refs;

    /* Index into the references info list. */
    uint64_t refs_start;

    /* Unmanaged size (memory held but not under the GC's contorl). */
    uint64_t unmanaged_size;
};

/* Reference identifier kinds. */
#define MVM_SNAPSHOT_REF_KIND_UNKNOWN   0
#define MVM_SNAPSHOT_REF_KIND_INDEX     1
#define MVM_SNAPSHOT_REF_KIND_STRING    2

/* Number of bits needed for ref kind. */
#define MVM_SNAPSHOT_REF_KIND_BITS      2

/* A reference from one collectable to another. */
struct MVMHeapSnapshotReference {
    /* The lower MVM_SNAPSHOT_REF_KIND_BITS bits indicate the type of reference.
     * After shifting those away, we either have a numeric index (e.g. for
     * array indexes) or an index into the string heap (for lexicals in frames
     * and attributes in objects). If kind is MVM_SNAPSHOT_REF_KIND_UNKNOWN the
     * rest of the bits will be zero; we know nothing of the relationship. */
    uint64_t description;

    /* The index of the collectable referenced. */
    uint64_t collectable_index;
};

/* Current state object whlie taking a heap snapshot. */
struct MVMHeapSnapshotState {
    /* The heap snapshot collection and current working snapshot. */
    MVMHeapSnapshotCollection *col;
    MVMHeapSnapshot *hs;

    /* Our current collectable worklist. */
    MVMHeapSnapshotWorkItem *workitems;
    uint64_t num_workitems;
    uint64_t alloc_workitems;

    /* The collectable we're currently adding references for. */
    uint64_t ref_from;

    /* The seen hash of collectables (including frames). */
    MVMPtrHashTable seen;

    /* We sometimes use GC mark functions to find references. Keep a worklist
     * around for those times (much cheaper than allocating it whenever we
     * need it). */
    MVMGCWorklist *gcwl;

    /* Many reprs have only one type (BOOTCode) or two
     * types (P6int, BOOTInt), so caching string index
     * per repr id is worth a lot. */
    uint64_t repr_str_idx_cache[MVM_REPR_MAX_COUNT];
    uint64_t type_str_idx_cache[MVM_REPR_MAX_COUNT];
    uint64_t anon_repr_type_str_idx_cache[MVM_REPR_MAX_COUNT];

    uint32_t type_of_type_idx_cache[8];
    uint32_t repr_of_type_idx_cache[8];
    uint32_t type_idx_cache[8];

    uint8_t type_idx_rotating_insert_slot;
};

/* Work item used while taking a heap snapshot. */
struct MVMHeapSnapshotWorkItem {
    /* The kind of collectable. */
    uint16_t kind;

    /* Index in the collectables (assigned upon adding to the worklist). */
    uint64_t col_idx;

    /* Target collectable, if any. */
    void *target;
};

int32_t MVM_profile_heap_profiling(struct MVMThreadContext *tc);
void MVM_profile_heap_start(struct MVMThreadContext *tc, MVMObject *config);
void MVM_profile_heap_take_snapshot(struct MVMThreadContext *tc);
MVMObject * MVM_profile_heap_end(struct MVMThreadContext *tc);

/* API for things that want to contribute more detailed data to the heap
 * profile. */
 void MVM_profile_heap_add_collectable_rel_const_cstr(struct MVMThreadContext *tc,
    MVMHeapSnapshotState *ss, MVMCollectable *collectable, const char *desc);
 void MVM_profile_heap_add_collectable_rel_const_cstr_cached(struct MVMThreadContext *tc,
    MVMHeapSnapshotState *ss, MVMCollectable *collectable, const char *desc, uint64_t *cache);
 void MVM_profile_heap_add_collectable_rel_vm_str(struct MVMThreadContext *tc,
    MVMHeapSnapshotState *ss, MVMCollectable *collectable, MVMString *desc);
 void MVM_profile_heap_add_collectable_rel_idx(struct MVMThreadContext *tc,
    MVMHeapSnapshotState *ss, MVMCollectable *collectable, uint64_t idx);
