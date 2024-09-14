/* Callsite argument flags. */
#define MVM_CALLSITE_ARG_TYPE_MASK       143
#define MVM_CALLSITE_ARG_NAMED_FLAT_MASK 159
typedef enum {
    /* Argument is an object. */
    MVM_CALLSITE_ARG_OBJ = 1,

    /* Argument is a native integer, signed. */
    MVM_CALLSITE_ARG_INT = 2,

    /* Argument is a native floating point number. */
    MVM_CALLSITE_ARG_NUM = 4,

    /* Argument is a native NFG string (MVMString REPR). */
    MVM_CALLSITE_ARG_STR = 8,

    /* Argument is a literal. */
    MVM_CALLSITE_ARG_LITERAL = 16,

    /* Argument is named. The name is placed in the MVMCallsite. */
    MVM_CALLSITE_ARG_NAMED = 32,

    /* Argument is flattened. What this means is up to the target. */
    MVM_CALLSITE_ARG_FLAT = 64,

    /* Argument is a native integer, unsigned. */
    MVM_CALLSITE_ARG_UINT = 128,
} MVMCallsiteFlags;

/* Callsites that are used within the VM. */
typedef enum {
    MVM_CALLSITE_ID_ZERO_ARITY,
    MVM_CALLSITE_ID_OBJ,
    MVM_CALLSITE_ID_STR,
    MVM_CALLSITE_ID_INT,
    MVM_CALLSITE_ID_OBJ_OBJ,
    MVM_CALLSITE_ID_OBJ_INT,
    MVM_CALLSITE_ID_OBJ_NUM,
    MVM_CALLSITE_ID_OBJ_STR,
    MVM_CALLSITE_ID_INT_INT,
    MVM_CALLSITE_ID_OBJ_OBJ_STR,
    MVM_CALLSITE_ID_OBJ_OBJ_OBJ,
} MVMCommonCallsiteID;

/* A callsite entry is just one of the above flags. */
typedef uint8_t MVMCallsiteEntry;

/* A callsite is an argument count, a bunch of flags, and names of named
 * arguments (excluding any flattening ones). Note that it does not contain
 * the argument values; this is the *statically known* things about the
 * callsite and is immutable. It describes how to process the callsite
 * memory buffer. */
struct MVMCallsite {
    /* The set of flags. */
    MVMCallsiteEntry *arg_flags;

    /* The number of arg flags. */
    uint16_t flag_count;

    /* The total argument count (including 2 for each named arg). */
    uint16_t arg_count;

    /* Number of positionals, including flattening positionals but
     * excluding named positionals. */
    uint16_t num_pos;

    /* Whether it has any flattening args. */
    uint8_t has_flattening;

    /* Whether it has been interned (which means it is suitable for using in
     * specialization). */
    uint8_t is_interned;

    /* Names of named arguments, in the order that they are passed (and thus
     * matching the flags). Note that named flattening args do not have an
     * entry here, even though they come in the nameds section. */
    MVMString **arg_names;
};

/* Maximum arity of a callsite (where arity is including positionals and
 * nameds in this case) that we immediately consider for interning. However,
 * we are willing to go over this if we are forced to intern something. */
#define MVM_INTERN_ARITY_SOFT_LIMIT     8

/* The growth size of the intern list for a given arity. */
#define MVM_INTERN_ARITY_GROW           8

/* Interned callsites data structure. */
struct MVMCallsiteInterns {
    /* 2-level array of pointers to callsites, first indexed by the arity,
     * then callsite of that arity. Freed at a safepoint if we grow beyond it. */
    MVMCallsite ***by_arity;

    /* Number of callsites we have interned by each arity. Also freed at a safepoint
     * on growth. */
    uint32_t *num_by_arity;

    /* The maximum interned arity so far (the element count of the above two
     * arrays). */
    uint32_t max_arity;
};

/* Functions relating to common callsites used within the VM. */
void MVM_callsite_initialize_common(struct MVMThreadContext *tc);
 MVMCallsite * MVM_callsite_get_common(struct MVMThreadContext *tc, MVMCommonCallsiteID id);

/* Other copying, interning, and cleanup. */
MVMCallsite * MVM_callsite_copy(struct MVMThreadContext *tc, const MVMCallsite *cs);
 void MVM_callsite_intern(struct MVMThreadContext *tc, MVMCallsite **cs,
        uint32_t force, uint32_t steal);
void MVM_callsite_mark(struct MVMThreadContext *tc, MVMCallsite *cs, MVMGCWorklist *worklist,
        MVMHeapSnapshotState *snapshot);
void MVM_callsite_mark_interns(struct MVMThreadContext *tc, MVMGCWorklist *worklist,
        MVMHeapSnapshotState *snapshot);
void MVM_callsite_destroy(MVMCallsite *cs);
void MVM_callsite_cleanup_interns(MVMInstance *instance);

/* Callsite transformations. */
MVMCallsite * MVM_callsite_drop_positional(struct MVMThreadContext *tc, MVMCallsite *cs, uint32_t idx);
MVMCallsite * MVM_callsite_drop_positionals(struct MVMThreadContext *tc, MVMCallsite *cs, uint32_t idx, uint32_t count);
MVMCallsite * MVM_callsite_insert_positional(struct MVMThreadContext *tc, MVMCallsite *cs, uint32_t idx,
        MVMCallsiteFlags flag);
MVMCallsite * MVM_callsite_replace_positional(struct MVMThreadContext *tc, MVMCallsite *cs, uint32_t idx,
        MVMCallsiteFlags flag);
/* Check if the callsite has nameds. */
static inline uint32_t MVM_callsite_has_nameds(struct MVMThreadContext *tc, const MVMCallsite *cs) {
    return cs->num_pos != cs->flag_count;
}

/* Count the number of nameds (excluding flattening). */
static inline uint16_t MVM_callsite_num_nameds(struct MVMThreadContext *tc, const MVMCallsite *cs) {
    uint16_t i = cs->num_pos;
    uint16_t nameds = 0;
    while (i < cs->flag_count) {
        if (!(cs->arg_flags[i] & MVM_CALLSITE_ARG_FLAT))
            nameds++;
        i++;
    }
    return nameds;
}

/* Describe the callsite flag type. */
static inline const char * MVM_callsite_arg_type_name(MVMCallsiteFlags f) {
    switch (f & MVM_CALLSITE_ARG_TYPE_MASK) {
        case MVM_CALLSITE_ARG_OBJ:
            return "obj";
        case MVM_CALLSITE_ARG_STR:
            return "str";
        case MVM_CALLSITE_ARG_INT:
            return "int";
        case MVM_CALLSITE_ARG_UINT:
            return "uint";
        case MVM_CALLSITE_ARG_NUM:
            return "num";
        default:
            return "unknown";
    }
}
