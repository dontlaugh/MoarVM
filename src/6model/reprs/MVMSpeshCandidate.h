/* A specialization candidate. */
struct MVMSpeshCandidateBody {
    /* The callsite that this specialization is for. */
    MVMCallsite *cs;

    /* The type tuple that this specialization is for, or NULL if it's a
     * certain specialization. */
    MVMSpeshStatsType *type_tuple;

    /* Has the candidated been discarded? */
    uint8_t discarded;

    /* Length of the specialized bytecode in bytes. */
    uint32_t bytecode_size;

    /* The specialized bytecode. */
    uint8_t *bytecode;

    /* Frame handlers for this specialization. */
    MVMFrameHandler *handlers;

    /* Spesh slots, used to hold information for fast access. */
    MVMCollectable **spesh_slots;

    /* Number of spesh slots. */
    uint32_t num_spesh_slots;

    /* The number of deoptimization mappings we have. */
    uint32_t num_deopts;

    /* Deoptimization mappings. */
    int32_t *deopts;

    /* Bit field of named args used to put in place during deopt, since we
     * typically don't update the array in specialized code. */
    uint64_t deopt_named_used_bit_field;

    /* Deopt information produced by escape analysis and scalar replacement. */
    MVMSpeshPEADeopt deopt_pea;

    /* Number of inlines and inlines table; see graph.h for description of
     * the table format. */
    uint32_t num_inlines;
    MVMSpeshInline *inlines;

    /* Resume initializations and the number of them. */
    MVMSpeshResumeInit *resume_inits;
    uint32_t num_resume_inits;

    /* The list of local types (only set up if we do inlines). */
    uint16_t *local_types;

    /* The list of lexical types (only set up if we do inlines). */
    uint16_t *lexical_types;

    /* Number of locals the specialized code has (may be different from the
     * original frame thanks to inlining). */
    uint16_t num_locals;

    /* Number of lexicals the specialized code has. */
    uint16_t num_lexicals;

    /* Memory sizes to allocate for work/env, taking into account inlining. */
    uint32_t work_size;
    uint32_t env_size;

    /* Number of handlers. */
    uint32_t num_handlers;

    /* JIT-code structure. */
    MVMJitCode *jitcode;

    /* Information used to reconstruct deoptimization usage info should we do
     * an inline of this candidate. It's stored as a sequence of integers of
     * the form:
     *  - Bytecode offset of writing instruction
     *  - Number of deopt indices that follow
     *  - The deopt indices
     *  There is a trailing -1 bytecode offset to mark the end of the data.
     */
    int32_t *deopt_usage_info;

    int32_t *deopt_synths;
    int32_t num_deopt_synths;
};

struct MVMSpeshCandidate {
    MVMObject common;
    MVMSpeshCandidateBody body;
};

/* Function for REPR setup. */
const MVMREPROps * MVMSpeshCandidate_initialize(MVMThreadContext *tc);

/* Functions for creating and clearing up specializations. */
void MVM_spesh_candidate_add(MVMThreadContext *tc, MVMSpeshPlanned *p);
void MVM_spesh_candidate_discard_existing(MVMThreadContext *tc, MVMStaticFrame *sf);
