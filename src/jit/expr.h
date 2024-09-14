

#define MVM_JIT_PTR_SZ sizeof(void*)
#define MVM_JIT_REG_SZ sizeof(MVMRegister)
#define MVM_JIT_INT_SZ sizeof(int64_t)
#define MVM_JIT_NUM_SZ sizeof(double)

/* C argument types */
enum {
    MVM_JIT_PTR,
    MVM_JIT_INT,
    MVM_JIT_NUM
};

#include "expr_ops.h"

enum MVMJitExprOperator {
#define MVM_JIT_OP_ENUM(name, nchild, narg) MVM_JIT_##name
MVM_JIT_EXPR_OPS(MVM_JIT_OP_ENUM)
#undef MVM_JIT_OP_ENUM
};

/* samcv++ for this trick */
#define MVM_STATIC_ASSERT(x) typedef char __ASSERT[(x)?1:-1]

/* Tree node information for easy access and use during compilation (a
   symbol table entry of sorts) */
struct MVMJitExprInfo {
    uint8_t num_links;
    uint8_t num_args;
    /* VM 'register' type represented by this node */
    int8_t type;
    /* Size of computed value */
    uint8_t size;
};

MVM_STATIC_ASSERT(sizeof(struct MVMJitExprInfo) <= sizeof(int32_t));

struct MVMJitExprTree {
    struct MVMJitGraph *graph;
    MVM_VECTOR_DECL(int32_t, nodes);
    MVM_VECTOR_DECL(int32_t, roots);
    MVM_VECTOR_DECL(union {
        int64_t i;
        double n;
        const void *p;
        uintptr_t u;
    }, constants);

    uint32_t seq_nr;
};

struct MVMJitExprTemplate {
    const int32_t *code;
    const char *info;
    int32_t len;
    int32_t root;
    int32_t flags;
};

#define MVM_JIT_EXPR_TEMPLATE_VALUE       0
#define MVM_JIT_EXPR_TEMPLATE_DESTRUCTIVE 1


struct MVMJitTreeTraverser {
    void  (*preorder)(struct MVMThreadContext *tc, struct MVMJitTreeTraverser *traverser,
                      struct MVMJitExprTree *tree, int32_t node);
    void   (*inorder)(struct MVMThreadContext *tc, struct MVMJitTreeTraverser *traverser,
                      struct MVMJitExprTree *tree, int32_t node, int32_t child);
    void (*postorder)(struct MVMThreadContext *tc, struct MVMJitTreeTraverser *traverser,
                      struct MVMJitExprTree *tree, int32_t node);
    void       *data;

    MVM_VECTOR_DECL(int32_t, visits);
    enum {
        MVM_JIT_TRAVERSER_REPEAT,
        MVM_JIT_TRAVERSER_ONCE
    } policy;

};



/* properties of expression ops */
enum MVMJitExprOperator MVM_jit_expr_op_invert_comparison(enum MVMJitExprOperator op);
int32_t MVM_jit_expr_op_is_binary(enum MVMJitExprOperator op);
int32_t MVM_jit_expr_op_is_unary(enum MVMJitExprOperator op);
int32_t MVM_jit_expr_op_is_commutative(enum MVMJitExprOperator op);
int32_t MVM_jit_expr_op_is_call(enum MVMJitExprOperator op);
const char * MVM_jit_expr_operator_name(struct MVMThreadContext *tc, enum MVMJitExprOperator operator);

struct MVMJitExprTree * MVM_jit_expr_tree_build(struct MVMThreadContext *tc, struct MVMJitGraph *jg, struct MVMSpeshIterator *iter);
int32_t MVM_jit_expr_apply_template(struct MVMThreadContext *tc, struct MVMJitExprTree *tree, const struct MVMJitExprTemplate*, int32_t *operands);
int32_t MVM_jit_expr_apply_template_adhoc(struct MVMThreadContext *tc, struct MVMJitExprTree *tree, char *template, ...);
void MVM_jit_expr_tree_traverse(struct MVMThreadContext *tc, struct MVMJitExprTree *tree, struct MVMJitTreeTraverser *traverser);
void MVM_jit_expr_tree_destroy(struct MVMThreadContext *tc, struct MVMJitExprTree *tree);
int32_t MVM_jit_expr_tree_get_nodes(struct MVMThreadContext *tc, struct MVMJitExprTree *tree,
                                     int32_t node, const char *path, int32_t *buffer);


static __inline__ struct MVMJitExprInfo * MVM_JIT_EXPR_INFO(struct MVMJitExprTree *tree, int32_t node) {
    return (struct MVMJitExprInfo*)(tree->nodes + node + 1);
}

static __inline__ uint8_t MVM_JIT_EXPR_NCHILD(struct MVMJitExprTree *tree, int32_t node) {
    return MVM_JIT_EXPR_INFO(tree, node)->num_links;
}

static __inline__ uint8_t MVM_JIT_EXPR_TYPE(struct MVMJitExprTree *tree, int32_t node) {
    return MVM_JIT_EXPR_INFO(tree, node)->type;
}

static __inline__ int32_t MVM_JIT_EXPR_FIRST_CHILD(struct MVMJitExprTree *tree, int32_t node) {
    return node + 2;
}

static __inline__ int32_t * MVM_JIT_EXPR_LINKS(struct MVMJitExprTree *tree, int32_t node) {
    return tree->nodes + MVM_JIT_EXPR_FIRST_CHILD(tree, node);
}

static __inline__ int32_t * MVM_JIT_EXPR_ARGS(struct MVMJitExprTree *tree, int32_t node) {
    return MVM_JIT_EXPR_LINKS(tree, node) + MVM_JIT_EXPR_NCHILD(tree, node);
}

static __inline__ int32_t MVM_JIT_EXPR_IS_NUM(struct MVMJitExprTree *tree, int32_t node) {
    uint8_t expr_type = MVM_JIT_EXPR_TYPE(tree,node);
    return expr_type == MVM_reg_num32 || expr_type == MVM_reg_num64;
}
