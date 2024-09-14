/* Container for arguments being passed to something to invoke it. */
struct MVMArgs {
    /* The callsite that describes the arguments. */
    MVMCallsite *callsite;

    /* A buffer we can obtain the arguments from. This is not a contiguous
     * list of arguments (at least, not in the common case!) In most cases,
     * it is the work area of the caller. */
    MVMRegister *source;

    /* A mapping array from argument number (according to flag index in the
     * callsite) to index in the source. Typically, this is a pointer into
     * the register list in the `dispatch` instruction. Thus to access arg 0
     * we'd do source[map[0]]. */
    uint16_t *map;
};

#define MVM_ARGS_SMALL_IDENTITY_MAP_SIZE    256
#define MVM_ARGS_LIMIT                      0xFFFF

/* Argument processing context. */
struct MVMArgProcContext {
    /* The incoming arguments to bind. */
    MVMArgs arg_info;

    /* Indexes of used nameds. If named_used_size is less than or equal to
     * 64, it will be a bit field. Otherwise, it will be a pointer to a
     * byte array. */
    union {
        uint8_t *byte_array;
        uint64_t bit_field;
    } named_used;
    uint16_t named_used_size;
};

/* Expected return type flags. */
typedef uint8_t MVMReturnType;
#define MVM_RETURN_VOID      0
#define MVM_RETURN_OBJ       1
#define MVM_RETURN_INT       2
#define MVM_RETURN_NUM       4
#define MVM_RETURN_STR       8
#define MVM_RETURN_UINT     32
/* When the thing returned to is "smart", like the debugserver,
 * set not only the return value but also the type depending on
 * which return_* op was used. */
#define MVM_RETURN_ALLOMORPH 16

/* Struct used for returning information about an argument. */
struct MVMArgInfo {
    MVMRegister arg;
    MVMCallsiteEntry   flags;
    uint8_t           exists;
    uint16_t          arg_idx; /* Set only for nameds, obvious for pos */
};

/* Initialize arguments processing context. */
static inline void MVM_args_proc_setup(struct MVMThreadContext *tc, MVMArgProcContext *ctx,
        MVMArgs arg_info) {
    ctx->arg_info = arg_info;
    uint16_t num_nameds = arg_info.callsite->flag_count - arg_info.callsite->num_pos;
    ctx->named_used_size = num_nameds;
    if (MVM_UNLIKELY(num_nameds > 64))
        ctx->named_used.byte_array = MVM_calloc(1, num_nameds);
    else
        ctx->named_used.bit_field = 0;
}

/* Clean up an arguments processing context. */
static inline void MVM_args_proc_cleanup(struct MVMThreadContext *tc, MVMArgProcContext *ctx) {
    if (ctx->named_used_size > 64) {
        MVM_free(ctx->named_used.byte_array);
        ctx->named_used_size = 0;
    }
}

/* Argument processing context handling. */
void MVM_args_setup_identity_map(struct MVMThreadContext *tc);
void MVM_args_grow_identity_map(struct MVMThreadContext *tc, MVMCallsite *callsite);
static inline uint16_t * MVM_args_identity_map(struct MVMThreadContext *tc,
        MVMCallsite *callsite) {
    if (callsite->flag_count > tc->instance->identity_arg_map_alloc)
        MVM_args_grow_identity_map(tc, callsite);
    return tc->instance->identity_arg_map;
}
void MVM_args_destroy_identity_map(struct MVMThreadContext *tc);
MVMCallStackFlattening * MVM_args_perform_flattening(struct MVMThreadContext *tc, MVMCallsite *cs,
        MVMRegister *source, uint16_t *map);

/* Arity checking and named handling. */
void MVM_args_checkarity(struct MVMThreadContext *tc, MVMArgProcContext *ctx, uint16_t min, uint16_t max);
void MVM_args_checkarity_for_jit(struct MVMThreadContext *tc, uint16_t min, uint16_t max);
void MVM_args_marked_named_used(struct MVMThreadContext *tc, uint32_t idx);
void MVM_args_throw_named_unused_error(struct MVMThreadContext *tc, MVMString *name);

/* Argument access by position. */
MVMObject * MVM_args_get_required_pos_obj(struct MVMThreadContext *tc, MVMArgProcContext *ctx, uint32_t pos);
MVMArgInfo MVM_args_get_optional_pos_obj(struct MVMThreadContext *tc, MVMArgProcContext *ctx, uint32_t pos);
int64_t MVM_args_get_required_pos_int(struct MVMThreadContext *tc, MVMArgProcContext *ctx, uint32_t pos);
MVMArgInfo MVM_args_get_optional_pos_int(struct MVMThreadContext *tc, MVMArgProcContext *ctx, uint32_t pos);
double MVM_args_get_required_pos_num(struct MVMThreadContext *tc, MVMArgProcContext *ctx, uint32_t pos);
MVMArgInfo MVM_args_get_optional_pos_num(struct MVMThreadContext *tc, MVMArgProcContext *ctx, uint32_t pos);
MVMString * MVM_args_get_required_pos_str(struct MVMThreadContext *tc, MVMArgProcContext *ctx, uint32_t pos);
MVMArgInfo MVM_args_get_optional_pos_str(struct MVMThreadContext *tc, MVMArgProcContext *ctx, uint32_t pos);
uint64_t MVM_args_get_required_pos_uint(struct MVMThreadContext *tc, MVMArgProcContext *ctx, uint32_t pos);
MVMArgInfo MVM_args_get_optional_pos_uint(struct MVMThreadContext *tc, MVMArgProcContext *ctx, uint32_t pos);
MVMObject * MVM_args_slurpy_positional(struct MVMThreadContext *tc, MVMArgProcContext *ctx, uint16_t pos);

/* Argument access by name. */
MVMArgInfo MVM_args_get_named_obj(struct MVMThreadContext *tc, MVMArgProcContext *ctx, MVMString *name, uint8_t required);
MVMArgInfo MVM_args_get_named_int(struct MVMThreadContext *tc, MVMArgProcContext *ctx, MVMString *name, uint8_t required);
MVMArgInfo MVM_args_get_named_num(struct MVMThreadContext *tc, MVMArgProcContext *ctx, MVMString *name, uint8_t required);
MVMArgInfo MVM_args_get_named_str(struct MVMThreadContext *tc, MVMArgProcContext *ctx, MVMString *name, uint8_t required);
MVMArgInfo MVM_args_get_named_uint(struct MVMThreadContext *tc, MVMArgProcContext *ctx, MVMString *name, uint8_t required);
MVMObject * MVM_args_slurpy_named(struct MVMThreadContext *tc, MVMArgProcContext *ctx);
int64_t MVM_args_has_named(struct MVMThreadContext *tc, MVMArgProcContext *ctx, MVMString *name);
void MVM_args_assert_nameds_used(struct MVMThreadContext *tc, MVMArgProcContext *ctx);

/* Result setting. */
void MVM_args_set_result_obj(struct MVMThreadContext *tc, MVMObject *result, int32_t frameless);
void MVM_args_set_result_int(struct MVMThreadContext *tc, int64_t result, int32_t frameless);
void MVM_args_set_result_uint(struct MVMThreadContext *tc, uint64_t result, int32_t frameless);
void MVM_args_set_result_num(struct MVMThreadContext *tc, double result, int32_t frameless);
void MVM_args_set_result_str(struct MVMThreadContext *tc, MVMString *result, int32_t frameless);
void MVM_args_assert_void_return_ok(struct MVMThreadContext *tc, int32_t frameless);
void MVM_args_set_dispatch_result_obj(struct MVMThreadContext *tc, MVMFrame *target, MVMObject *result);
void MVM_args_set_dispatch_result_int(struct MVMThreadContext *tc, MVMFrame *target, int64_t result);
void MVM_args_set_dispatch_result_uint(struct MVMThreadContext *tc, MVMFrame *target, uint64_t result);
void MVM_args_set_dispatch_result_num(struct MVMThreadContext *tc, MVMFrame *target, double result);
void MVM_args_set_dispatch_result_str(struct MVMThreadContext *tc, MVMFrame *target, MVMString *result);

/* Capture handling. */
MVMCallsite * MVM_args_copy_callsite(struct MVMThreadContext *tc, MVMArgProcContext *ctx);
 MVMObject * MVM_args_use_capture(struct MVMThreadContext *tc, MVMFrame *f);
 MVMObject * MVM_args_save_capture(struct MVMThreadContext *tc, MVMFrame *f);

/* Custom bind failure/success handling. */
void MVM_args_bind_failed(struct MVMThreadContext *tc, MVMDispInlineCacheEntry **ice_ptr);
void MVM_args_bind_succeeded(struct MVMThreadContext *tc, MVMDispInlineCacheEntry **ice_ptr);

/* Result setting frame constants. */
#define MVM_RETURN_CALLER_FRAME     0
#define MVM_RETURN_CURRENT_FRAME    1

/* Required/optional constants. */
#define MVM_ARG_OPTIONAL    0
#define MVM_ARG_REQUIRED    1
