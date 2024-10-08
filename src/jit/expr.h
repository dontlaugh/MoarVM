#ifndef EXPR_H
#define EXPR_H
#define MVM_JIT_PTR_SZ sizeof(void*)
#define MVM_JIT_REG_SZ sizeof(MVMRegister)
#define MVM_JIT_INT_SZ sizeof(MVMint64)
#define MVM_JIT_NUM_SZ sizeof(MVMnum64)

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
    MVMuint8 num_links;
    MVMuint8 num_args;
    /* VM 'register' type represented by this node */
    MVMint8 type;
    /* Size of computed value */
    MVMuint8 size;
};

MVM_STATIC_ASSERT(sizeof(MVMJitExprInfo) <= sizeof(MVMint32));

struct MVMJitExprTree {
    MVMJitGraph *graph;
    MVM_VECTOR_DECL(MVMint32, nodes);
    MVM_VECTOR_DECL(MVMint32, roots);
    MVM_VECTOR_DECL(union {
        MVMint64 i;
        MVMnum64 n;
        const void *p;
        uintptr_t u;
    }, constants);

    MVMuint32 seq_nr;
};

struct MVMJitExprTemplate {
    const MVMint32 *code;
    const char *info;
    MVMint32 len;
    MVMint32 root;
    MVMint32 flags;
};

#define MVM_JIT_EXPR_TEMPLATE_VALUE       0
#define MVM_JIT_EXPR_TEMPLATE_DESTRUCTIVE 1


struct MVMJitTreeTraverser {
    void  (*preorder)(MVMThreadContext *tc, MVMJitTreeTraverser *traverser,
                      MVMJitExprTree *tree, MVMint32 node);
    void   (*inorder)(MVMThreadContext *tc, MVMJitTreeTraverser *traverser,
                      MVMJitExprTree *tree, MVMint32 node, MVMint32 child);
    void (*postorder)(MVMThreadContext *tc, MVMJitTreeTraverser *traverser,
                      MVMJitExprTree *tree, MVMint32 node);
    void       *data;

    MVM_VECTOR_DECL(MVMint32, visits);
    enum {
        MVM_JIT_TRAVERSER_REPEAT,
        MVM_JIT_TRAVERSER_ONCE
    } policy;

};



/* properties of expression ops */
enum MVMJitExprOperator MVM_jit_expr_op_invert_comparison(enum MVMJitExprOperator op);
MVMint32 MVM_jit_expr_op_is_binary(enum MVMJitExprOperator op);
MVMint32 MVM_jit_expr_op_is_unary(enum MVMJitExprOperator op);
MVMint32 MVM_jit_expr_op_is_commutative(enum MVMJitExprOperator op);
MVMint32 MVM_jit_expr_op_is_call(enum MVMJitExprOperator op);
const char * MVM_jit_expr_operator_name(MVMThreadContext *tc, enum MVMJitExprOperator operator);

MVMJitExprTree * MVM_jit_expr_tree_build(MVMThreadContext *tc, MVMJitGraph *jg, MVMSpeshIterator *iter);
MVMint32 MVM_jit_expr_apply_template(MVMThreadContext *tc, MVMJitExprTree *tree, const MVMJitExprTemplate*, MVMint32 *operands);
MVMint32 MVM_jit_expr_apply_template_adhoc(MVMThreadContext *tc, MVMJitExprTree *tree, char *template, ...);
void MVM_jit_expr_tree_traverse(MVMThreadContext *tc, MVMJitExprTree *tree, MVMJitTreeTraverser *traverser);
void MVM_jit_expr_tree_destroy(MVMThreadContext *tc, MVMJitExprTree *tree);
MVMint32 MVM_jit_expr_tree_get_nodes(MVMThreadContext *tc, MVMJitExprTree *tree,
                                     MVMint32 node, const char *path, MVMint32 *buffer);


MVM_STATIC_INLINE MVMJitExprInfo * MVM_JIT_EXPR_INFO(MVMJitExprTree *tree, MVMint32 node) {
    return (MVMJitExprInfo*)(tree->nodes + node + 1);
}

MVM_STATIC_INLINE MVMuint8 MVM_JIT_EXPR_NCHILD(MVMJitExprTree *tree, MVMint32 node) {
    return MVM_JIT_EXPR_INFO(tree, node)->num_links;
}

MVM_STATIC_INLINE MVMuint8 MVM_JIT_EXPR_TYPE(MVMJitExprTree *tree, MVMint32 node) {
    return MVM_JIT_EXPR_INFO(tree, node)->type;
}

MVM_STATIC_INLINE MVMint32 MVM_JIT_EXPR_FIRST_CHILD(MVMJitExprTree *tree, MVMint32 node) {
    return node + 2;
}

MVM_STATIC_INLINE MVMint32 * MVM_JIT_EXPR_LINKS(MVMJitExprTree *tree, MVMint32 node) {
    return tree->nodes + MVM_JIT_EXPR_FIRST_CHILD(tree, node);
}


MVM_STATIC_INLINE MVMint32 * MVM_JIT_EXPR_ARGS(MVMJitExprTree *tree, MVMint32 node) {
    return MVM_JIT_EXPR_LINKS(tree, node) + MVM_JIT_EXPR_NCHILD(tree, node);
}

MVM_STATIC_INLINE MVMint32 MVM_JIT_EXPR_IS_NUM(MVMJitExprTree *tree, MVMint32 node) {
    MVMuint8 expr_type = MVM_JIT_EXPR_TYPE(tree,node);
    return expr_type == MVM_reg_num32 || expr_type == MVM_reg_num64;
}
#endif // EXPR_H
