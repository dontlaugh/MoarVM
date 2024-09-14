/* A GC sync point is a point where we can check if we're being signalled
 * to stop to do a GC run. This is placed at points where it is safe to
 * do such a thing, and hopefully so that it happens often enough; note
 * that every call down to the allocator is also a sync point, so this
 * really only means we need to do this enough to make sure tight native
 * loops trigger it. */
/* Don't use a MVM_load(&tc->gc_status) here for performance, it's okay
 * if the interrupt is delayed a bit. */
#define GC_SYNC_POINT(tc) \
    if (tc->gc_status) { \
        MVM_gc_enter_from_interrupt(tc); \
    }

/* Different views of a register. */
union MVMRegister {
    MVMObject         *o;
    MVMString *s;
    int8_t            i8;
    uint8_t           u8;
    int16_t           i16;
    uint16_t          u16;
    int32_t           i32;
    uint32_t          u32;
    int64_t           i64;
    uint64_t          u64;
    float           n32;
    double           n64;
};

/* Most operands an operation will have. */
#define MVM_MAX_OPERANDS 8

/* Kind of de-opt mark. */
#define MVM_DEOPT_MARK_ONE      1
#define MVM_DEOPT_MARK_ALL      2
#define MVM_DEOPT_MARK_OSR      4
#define MVM_DEOPT_MARK_ONE_PRE  8

/* Information about an opcode. */
struct MVMOpInfo {
    uint16_t   opcode;
    const char *name;
    uint16_t   num_operands;
    uint8_t    pure : 1;
    uint8_t    deopt_point : 4;
    uint8_t    may_cause_deopt : 1;
    uint8_t    logged : 1;
    uint8_t    post_logged : 1;
    uint8_t    no_inline : 1;
    uint8_t    jittivity : 2;
    uint8_t    uses_hll : 1;
    uint8_t    specializable : 1;
    uint8_t    uses_cache : 1;
    uint8_t    operands[MVM_MAX_OPERANDS];
};

/* Operand read/write/literal flags. */
#define MVM_operand_literal     0
#define MVM_operand_read_reg    1
#define MVM_operand_write_reg   2
#define MVM_operand_read_lex    3
#define MVM_operand_write_lex   4
#define MVM_operand_rw_mask     7

/* Register data types. */
#define MVM_reg_int8            1
#define MVM_reg_int16           2
#define MVM_reg_int32           3
#define MVM_reg_int64           4
#define MVM_reg_num32           5
#define MVM_reg_num64           6
#define MVM_reg_str             7
#define MVM_reg_obj             8
#define MVM_reg_uint8           17
#define MVM_reg_uint16          18
#define MVM_reg_uint32          19
#define MVM_reg_uint64          20

/* Operand data types. */
#define MVM_operand_int8        (MVM_reg_int8 << 3)
#define MVM_operand_int16       (MVM_reg_int16 << 3)
#define MVM_operand_int32       (MVM_reg_int32 << 3)
#define MVM_operand_int64       (MVM_reg_int64 << 3)
#define MVM_operand_num32       (MVM_reg_num32 << 3)
#define MVM_operand_num64       (MVM_reg_num64 << 3)
#define MVM_operand_str         (MVM_reg_str << 3)
#define MVM_operand_obj         (MVM_reg_obj << 3)
#define MVM_operand_ins         (9 << 3)
#define MVM_operand_type_var    (10 << 3)
#define MVM_operand_coderef     (12 << 3)
#define MVM_operand_callsite    (13 << 3)
#define MVM_operand_spesh_slot  (16 << 3)
#define MVM_operand_uint8       (MVM_reg_uint8 << 3)
#define MVM_operand_uint16      (MVM_reg_uint16 << 3)
#define MVM_operand_uint32      (MVM_reg_uint32 << 3)
#define MVM_operand_uint64      (MVM_reg_uint64 << 3)
#define MVM_operand_type_mask   (31 << 3)

#ifdef MVM_BIGENDIAN
#define MVM_SWITCHENDIAN 1
#else
#define MVM_SWITCHENDIAN 2
#endif

struct MVMRunloopState {
    uint8_t **interp_cur_op;
    uint8_t **interp_bytecode_start;
    MVMRegister **interp_reg_base;
    MVMCompUnit **interp_cu;
};

/* Functions. */
void MVM_interp_run(struct MVMThreadContext *tc, void (*initial_invoke)(struct MVMThreadContext *, void *), void *invoke_data, MVMRunloopState *outer_runloop);
void MVM_interp_run_nested(struct MVMThreadContext *tc, void (*initial_invoke)(struct MVMThreadContext *, void *), void *invoke_data, MVMRegister *res);
 void MVM_interp_enable_tracing(void);

static inline int64_t MVM_BC_get_I64(const uint8_t *cur_op, int offset) {
    const uint8_t *const where = cur_op + offset;
#ifdef MVM_CAN_UNALIGNED_INT64
    return *(int64_t *)where;
#else
    int64_t temp;
    memmove(&temp, where, sizeof(int64_t));
    return temp;
#endif
}

static inline double MVM_BC_get_N64(const uint8_t *cur_op, int offset) {
#ifdef MVM_CAN_UNALIGNED_NUM64
    const uint8_t *const where = cur_op + offset;
    return *(double *)where;
#else
    double temp;
    memmove(&temp, cur_op + offset, sizeof(double));
    return temp;
#endif
}
/* For MVM_reg_* types */
static char * MVM_reg_get_debug_name(struct MVMThreadContext *tc, uint16_t type) {
    switch (type) {
        case MVM_reg_int8:
            return "int8";
        case MVM_reg_int16:
            return "int16";
        case MVM_reg_int32:
            return "int32";
        case MVM_reg_int64:
            return "int64";
        case MVM_reg_num32:
            return "num32";
        case MVM_reg_num64:
            return "num64";
        case MVM_reg_str:
            return "str";
        case MVM_reg_obj:
            return "obj";
        case MVM_reg_uint8:
            return "uint8";
        case MVM_reg_uint16:
            return "uint16";
        case MVM_reg_uint32:
            return "uint32";
        case MVM_reg_uint64:
            return "uint64";
        default:
            return "unknown";
    }
}
