#define MVMPhiNodeCacheSize             48
#define MVMPhiNodeCacheSparseBegin      32

/* Top level of a spesh graph, representing a particular static frame (and
 * potentially having others inlined into it). */
struct MVMSpeshGraph {
    /* The static frame this is the spesh graph for. */
    MVMStaticFrame *sf;

    /* The callsite this spesh graph has been tailored to. */
    MVMCallsite *cs;

    /* The bytecode we're building the graph out of. */
    uint8_t *bytecode;

    /* Exception handler map for that bytecode. */
    MVMFrameHandler *handlers;

    /* Handlers that have become unreachable due to dead code removal. */
    int8_t *unreachable_handlers;

    /* The size of the bytecode we're building the graph out of. */
    uint32_t bytecode_size;

    /* Number of exception handlers. */
    uint32_t num_handlers;

    /* The entry basic block. */
    MVMSpeshBB *entry;

    /* Gathered facts about each version of a local (top-level array is per
     * local, then array hanging off it is per version). */
    MVMSpeshFacts **facts;

    /* Number of fact entries per local. */
    uint16_t *fact_counts;

    /* Log-based guards added. */
    MVMSpeshLogGuard *log_guards;

    /* Number of log-based guards we have. */
    int32_t num_log_guards;

    /* Region allocator for spesh nodes */
    MVMRegionAlloc region_alloc;

    /* Values placed in spesh slots. */
    MVMCollectable **spesh_slots;

    /* Number of spesh slots we have used and allocated. */
    uint16_t num_spesh_slots;
    uint16_t alloc_spesh_slots;

    /* De-opt indexes, as pairs of integers. The first integer, set when we
     * build the graph, is the return address in the original bytecode. The
     * code-gen phase for the specialized bytecode will fill in the second
     * integers afterwards, which are the return address in the specialized
     * bytecode. */
    int32_t *deopt_addrs;
    uint32_t num_deopt_addrs;
    uint32_t alloc_deopt_addrs;

    /* Bit field of named args used to put in place during deopt, since we
     * don't typically don't update the array in specialized code. */
    uint64_t deopt_named_used_bit_field;

    /* Deopt points that are considered to be always in use, even if they
     * are not on a deopting instruction. */
    uint32_t *always_retained_deopt_idxs;
    uint32_t num_always_retained_deopt_idxs;

    /* Deopt information produced by escape analysis and scalar replacement. */
    MVMSpeshPEADeopt deopt_pea;

    /* Table of information about inlines, laid out in order of nesting
     * depth. Thus, going through the table in order and finding when we
     * are within the bounds will show up each call frame that needs to
     * be created in deopt. */
    MVMSpeshInline *inlines;
    uint32_t num_inlines;

    /* Table of information about resume initialization arguments, if any. */
    MVM_VECTOR_DECL(MVMSpeshResumeInit, resume_inits);

    /* Number of basic blocks we have. */
    uint32_t num_bbs;

    /* The list of local types (only set up if we do inlines). */
    uint16_t *local_types;

    /* The list of lexical types (only set up if we do inlines). */
    uint16_t *lexical_types;

    /* The total number of locals, accounting for any inlining done and
     * added temporaries. */
    uint16_t num_locals;

    /* The total number of lexicals, accounting for any inlining done. */
    uint16_t num_lexicals;

    /* Temporary local registers added to aid transformations, along with a
     * count of the number we have and have allocated space for so far. */
    uint16_t          num_temps;
    uint16_t          alloc_temps;
    MVMSpeshTemporary *temps;

    /* We need to create new MVMOpInfo structs for each number of
     * arguments a PHI node can take. We cache them here, so that we
     * allocate fewer of them across our spesh alloc blocks.
     */
    MVMOpInfo *phi_infos;

    /* If this graph was formed from a spesh candidate rather than an
     * original static frame, the candidate will be stored here. */
    MVMSpeshCandidate *cand;

    /* Did we specialize on the invocant type? */
    uint8_t specialized_on_invocant;

    /* Stored in comment annotations to give an ordering of comments */
    uint32_t next_annotation_idx;
};

/* A temporary register, added to support transformations. */
struct MVMSpeshTemporary {
    /* The number of the local along with the current SSA index. */
    uint16_t orig;
    uint16_t i;

    /* The SSA index currently loaned out. */
    uint16_t used_i;

    /* What kind of register is it? */
    uint16_t kind;

    /* Is it currently in use? */
    uint16_t in_use;
};

/* A basic block in the graph (sequences of instructions where control will
 * always enter at the start and leave at the end). */
struct MVMSpeshBB {
    /* Head/tail of doubly linked list of instructions. */
    MVMSpeshIns *first_ins;
    MVMSpeshIns *last_ins;

    /* Basic blocks we may go to after this one. */
    MVMSpeshBB **succ;

    /* Basic blocks that we may arrive into this one from. */
    MVMSpeshBB **pred;

    /* Children in the dominator tree. */
    MVMSpeshBB **children;

    /* Dominance frontier set. */
    MVMSpeshBB **df;

    /* Basic blocks that we may go to if we throw. */
    MVMSpeshBB **handler_succ;

    /* Counts for the above, grouped together to avoid alignment holes. */
    uint16_t    num_succ;
    uint16_t    num_pred;
    uint16_t    num_children;
    uint16_t    num_df;
    uint16_t    num_handler_succ;

    /* The next basic block in original linear code order. */
    MVMSpeshBB *linear_next;

    /* Index (just an ascending integer along the linear_next chain), used as
     * the block identifier in dominance computation and for debug output. */
    int32_t idx;

    /* The block's reverse post-order index, assinged when computing
     * dominance. */
    int32_t rpo_idx;

    /* We cache the instruction pointer of the very first instruction so that
     * we can output a line number for every BB */
    uint32_t initial_pc;

    /* Is this block an inlining of another one? */
    int8_t inlined;

    /* Is this basic block part of a jump list? */
    int8_t jumplist;

    /* Is this basic block dead (removed due to being unreachable)? */
    int8_t dead;
};

/* The SSA phi instruction. */
#define MVM_SSA_PHI 32767

/* An instruction in the spesh graph. */
struct MVMSpeshIns {
    /* Instruction information. */
    const MVMOpInfo *info;

    /* Operand information. */
    MVMSpeshOperand *operands;

    /* Previous and next instructions, within a basic block boundary. */
    MVMSpeshIns *prev;
    MVMSpeshIns *next;

    /* Any annotations on the instruction. */
    MVMSpeshAnn *annotations;
};

/* Union type of operands in a spesh instruction; the op info and phase of the
 * optimizer we're in determines which of these we look at. */
union MVMSpeshOperand {
    int64_t     lit_i64;
    int64_t     lit_ui64;
    int32_t     lit_i32;
    uint32_t    lit_ui32;
    int16_t     lit_i16;
    uint16_t    lit_ui16;
    int8_t      lit_i8;
    double     lit_n64;
    float     lit_n32;
    uint32_t    lit_str_idx;
    uint16_t    callsite_idx;
    uint16_t    coderef_idx;
    uint32_t    ins_offset;
    MVMSpeshBB  *ins_bb;
    struct {
        uint16_t idx;
        uint16_t outers;
    } lex;
    struct {
        int32_t  i;    /* SSA-computed version. */
        uint16_t orig; /* Original register number. */
    } reg;
};

/* Annotations base. */
struct MVMSpeshAnn {
    /* The next annotation in the chain, if any. */
    MVMSpeshAnn *next;

    /* The type of annotation we have. */
    int32_t type;

    /* Order in which the annotation was created; only used
     * for comments right now, but put in front of the union
     * as there is a 4 byte hole here otherwise. */
    uint32_t order;

    /* Data (meaning depends on type). */
    union {
        uint32_t frame_handler_index;
        int32_t deopt_idx;
        int32_t inline_idx;
        uint32_t bytecode_offset;
        struct {
            uint32_t filename_string_index;
            uint32_t line_number;
        } lineno;
        char *comment;
        MVMSpeshOperand *temps_to_release;
    } data;
};

/* Annotation types. */
#define MVM_SPESH_ANN_FH_START      1
#define MVM_SPESH_ANN_FH_END        2
#define MVM_SPESH_ANN_FH_GOTO       3
#define MVM_SPESH_ANN_DEOPT_ONE_INS 4
#define MVM_SPESH_ANN_DEOPT_ALL_INS 5
#define MVM_SPESH_ANN_INLINE_START  6
#define MVM_SPESH_ANN_INLINE_END    7
#define MVM_SPESH_ANN_DEOPT_INLINE  8
#define MVM_SPESH_ANN_DEOPT_OSR     9
#define MVM_SPESH_ANN_LINENO        10
#define MVM_SPESH_ANN_LOGGED        11
#define MVM_SPESH_ANN_DEOPT_SYNTH   12
#define MVM_SPESH_ANN_CACHED        13
#define MVM_SPESH_ANN_DEOPT_PRE_INS 14
#define MVM_SPESH_ANN_DELAYED_TEMPS 2048
#define MVM_SPESH_ANN_COMMENT       4096

/* Functions to create/destroy the spesh graph. */
MVMSpeshGraph * MVM_spesh_graph_create(struct MVMThreadContext *tc, MVMStaticFrame *sf,
    uint32_t cfg_only, uint32_t insert_object_nulls);
MVMSpeshGraph * MVM_spesh_graph_create_from_cand(struct MVMThreadContext *tc, MVMStaticFrame *sf,
    MVMSpeshCandidate *cand, uint32_t cfg_only, MVMSpeshIns ***deopt_usage_ins_out);
MVMSpeshBB * MVM_spesh_graph_linear_prev(struct MVMThreadContext *tc, MVMSpeshGraph *g, MVMSpeshBB *search);
void MVM_spesh_graph_grow_deopt_table(struct MVMThreadContext *tc, MVMSpeshGraph *g);
int32_t MVM_spesh_graph_add_deopt_annotation(struct MVMThreadContext *tc, MVMSpeshGraph *g,
    MVMSpeshIns *ins_node, uint32_t deopt_target, int32_t type);
MVMSpeshBB ** MVM_spesh_graph_reverse_postorder(struct MVMThreadContext *tc, MVMSpeshGraph *g);
void MVM_spesh_graph_recompute_dominance(struct MVMThreadContext *tc, MVMSpeshGraph *g);
void MVM_spesh_graph_mark(struct MVMThreadContext *tc, MVMSpeshGraph *g, MVMGCWorklist *worklist);
void MVM_spesh_graph_describe(struct MVMThreadContext *tc, MVMSpeshGraph *g, MVMHeapSnapshotState *snapshot);
void MVM_spesh_graph_destroy(struct MVMThreadContext *tc, MVMSpeshGraph *g);
 void * MVM_spesh_alloc(struct MVMThreadContext *tc, MVMSpeshGraph *g, size_t bytes);
MVMOpInfo *MVM_spesh_graph_get_phi(struct MVMThreadContext *tc, MVMSpeshGraph *g, uint32_t nrargs);
void MVM_spesh_graph_place_phi(struct MVMThreadContext *tc, MVMSpeshGraph *g, MVMSpeshBB *bb, int32_t n, uint16_t var);

 void MVM_spesh_graph_add_comment(struct MVMThreadContext *tc, MVMSpeshGraph *g,
    MVMSpeshIns *ins, const char *fmt, ...);

static inline uint32_t MVM_spesh_is_inc_dec_op(uint16_t opcode) {
    return opcode == MVM_OP_inc_i || opcode == MVM_OP_dec_i ||
           opcode == MVM_OP_inc_u || opcode == MVM_OP_dec_u;
}
int MVM_spesh_graph_ins_ends_bb(struct MVMThreadContext *tc, const MVMOpInfo *info);
