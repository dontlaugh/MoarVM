struct MVMJitTileTemplate {
    void (*emit)(MVMThreadContext *tc, MVMJitCompiler *compiler, MVMJitTile *tile, MVMJitExprTree *tree);
    const char    *path;
    const char    *expr;
    int32_t  left_sym;
    int32_t  right_sym;

    uint32_t  num_refs;
    uint32_t value_bitmap;
    MVMuint8 register_spec[4];
};

struct MVMJitTile {
    void (*emit)(MVMThreadContext *tc, MVMJitCompiler *compiler, MVMJitTile *tile, MVMJitExprTree *tree);
    int32_t node;
    enum MVMJitExprOperator op;

    uint32_t  num_refs;
    int32_t   refs[4];
    int32_t   args[6];
    MVMuint8 values[4];

    MVMuint8 register_spec[4];
    MVMint8   size;

    const char *debug_name;
};

struct MVMJitTileBB {
    /* first and last tile index of code  */
    uint32_t start, end;
    /* up to two successors */
    uint32_t num_succ, succ[2];
};

/* A tile I'm planning to insert into the list */
struct MVMJitTileInsert {
    uint32_t position;
    int32_t order;
    MVMJitTile *tile;
};

/* A list of tiles representing a (part of a) routine */
struct MVMJitTileList {
    MVMJitExprTree *tree;
    MVM_VECTOR_DECL(MVMJitTile*, items);
    MVM_VECTOR_DECL(struct MVMJitTileInsert, inserts);
    MVM_VECTOR_DECL(struct MVMJitTileBB, blocks);

    /* TODO implement structures to mark basic blocks */
    int32_t num_arglist_refs;
};




MVMJitTile     * MVM_jit_tile_make(MVMThreadContext *tc, MVMJitCompiler *compiler, void *emit,
                                   int32_t num_args, int32_t num_values, ...);
MVMJitTile     * MVM_jit_tile_make_from_template(MVMThreadContext *tc, MVMJitCompiler *compiler,
                                                 const MVMJitTileTemplate *template,
                                                 MVMJitExprTree *tree, int32_t node);
MVMJitTileList * MVM_jit_tile_expr_tree(MVMThreadContext *tc, MVMJitCompiler *compiler, MVMJitExprTree *tree);


void MVM_jit_tile_list_insert(MVMThreadContext *tc, MVMJitTileList *list, MVMJitTile *tile, uint32_t position, int32_t order);
void MVM_jit_tile_list_edit(MVMThreadContext *tc, MVMJitTileList *list);
void MVM_jit_tile_list_destroy(MVMThreadContext *tc, MVMJitTileList *list);

#define MVM_JIT_TILE_YIELDS_VALUE(t) (MVM_JIT_REGISTER_IS_USED(t->register_spec[0]))

#define MVM_JIT_TILE_NAME(name) MVM_jit_tile_ ## name
#define MVM_JIT_TILE_DECL(name) \
    void MVM_JIT_TILE_NAME(name) (MVMThreadContext *tc, MVMJitCompiler *compiler, MVMJitTile *tile, MVMJitExprTree *tree)
