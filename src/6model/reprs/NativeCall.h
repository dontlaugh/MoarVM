/* Representation for a native callsite. */
struct MVMNativeCallBody {
    char       *lib_name;
    DLLib      *lib_handle;
    char       *sym_name;
    void       *entry_point;
#ifdef HAVE_LIBFFI
    ffi_abi     convention;
    ffi_type  **ffi_arg_types;
    ffi_type   *ffi_ret_type;
#else
    int16_t    convention;
#endif
    int16_t    num_args;
    int16_t    ret_type;
    int16_t   *arg_types;
    MVMObject **arg_info;
    MVMCode    *resolve_lib_name;
    MVMObject  *resolve_lib_name_arg;
};

struct MVMNativeCall {
    MVMObject common;
    MVMNativeCallBody body;
};

/* Function for REPR setup. */
const MVMREPROps * MVMNativeCall_initialize(struct MVMThreadContext *tc);
