typedef void MVMExtOpFunc(struct MVMThreadContext *tc, uint8_t *cur_op);
typedef void MVMExtOpSpesh(struct MVMThreadContext *tc, MVMSpeshGraph *g, MVMSpeshBB *bb, MVMSpeshIns *ins);
typedef void MVMExtOpFactDiscover(struct MVMThreadContext *tc, MVMSpeshGraph *g, MVMSpeshIns *ins);

/* Flags we might put on an extension op to indicate its properties. */
#define MVM_EXTOP_PURE          1
#define MVM_EXTOP_NOINLINE      2
#define MVM_EXTOP_INVOKISH      4
#define MVM_EXTOP_NO_JIT        8
#define MVM_EXTOP_ALLOCATING    16

struct MVMExtRegistry {
    MVMString *hash_key;
    MVMDLLSym *sym;
};

struct MVMExtOpRegistry {
    MVMString *hash_key;
    MVMExtOpFunc *func;
    MVMOpInfo info;
    MVMExtOpSpesh *spesh;
    MVMExtOpFactDiscover *discover;
    uint32_t no_jit;
    uint32_t allocating;
};

int MVM_ext_load(struct MVMThreadContext *tc, MVMString *lib, MVMString *ext);
 int MVM_ext_register_extop(struct MVMThreadContext *tc, const char *cname,
        MVMExtOpFunc func, uint8_t num_operands, uint8_t operands[],
        MVMExtOpSpesh *spesh, MVMExtOpFactDiscover *discover, uint32_t flags);
const MVMOpInfo * MVM_ext_resolve_extop_record(struct MVMThreadContext *tc,
        MVMExtOpRecord *record);
const MVMOpInfo * MVM_ext_resolve_extop_record_in_cu(struct MVMThreadContext *tc,
        MVMCompUnit *cu, uint16_t opcode);
