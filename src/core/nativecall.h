/* The various native call argument types. */
#define MVM_NATIVECALL_ARG_VOID            0
#define MVM_NATIVECALL_ARG_CHAR            2
#define MVM_NATIVECALL_ARG_SHORT           4
#define MVM_NATIVECALL_ARG_INT             6
#define MVM_NATIVECALL_ARG_LONG            8
#define MVM_NATIVECALL_ARG_LONGLONG        10
#define MVM_NATIVECALL_ARG_FLOAT           12
#define MVM_NATIVECALL_ARG_DOUBLE          14
#define MVM_NATIVECALL_ARG_ASCIISTR        16
#define MVM_NATIVECALL_ARG_UTF8STR         18
#define MVM_NATIVECALL_ARG_UTF16STR        20
#define MVM_NATIVECALL_ARG_CSTRUCT         22
#define MVM_NATIVECALL_ARG_CARRAY          24
#define MVM_NATIVECALL_ARG_CALLBACK        26
#define MVM_NATIVECALL_ARG_CPOINTER        28
#define MVM_NATIVECALL_ARG_VMARRAY         30
#define MVM_NATIVECALL_ARG_UCHAR           32
#define MVM_NATIVECALL_ARG_USHORT          34
#define MVM_NATIVECALL_ARG_UINT            36
#define MVM_NATIVECALL_ARG_ULONG           38
#define MVM_NATIVECALL_ARG_ULONGLONG       40
#define MVM_NATIVECALL_ARG_CUNION          42
#define MVM_NATIVECALL_ARG_CPPSTRUCT       44
#define MVM_NATIVECALL_ARG_TYPE_MASK       62

/* Flag for whether we should free a string after passing it or not. */
#define MVM_NATIVECALL_ARG_NO_FREE_STR     0
#define MVM_NATIVECALL_ARG_FREE_STR        1
#define MVM_NATIVECALL_ARG_FREE_STR_MASK   1
/* Flag for whether we need to refresh a CArray after passing or not. */
#define MVM_NATIVECALL_ARG_NO_REFRESH      0
#define MVM_NATIVECALL_ARG_REFRESH         1
#define MVM_NATIVECALL_ARG_REFRESH_MASK    1
#define MVM_NATIVECALL_ARG_NO_RW           0
#define MVM_NATIVECALL_ARG_RW              256
#define MVM_NATIVECALL_ARG_RW_MASK         256

#define MVM_NATIVECALL_UNMARSHAL_KIND_GENERIC     -1
#define MVM_NATIVECALL_UNMARSHAL_KIND_RETURN      -2
#define MVM_NATIVECALL_UNMARSHAL_KIND_NATIVECAST  -3

/* Native callback entry. Hung off MVMNativeCallbackCacheHead, which is
 * a hash owned by the ThreadContext. All MVMNativeCallbacks in a linked
 * list have the same cuid, which is the key to the CacheHead hash.
 */
struct MVMNativeCallback {
    /* The dyncall/libffi callback object. */
    void *cb;

    /* The code handle that we will call. */
    MVMCode *target;

    /* The VM instance. */
    MVMInstance *instance;

    /* Return and argument type flags. */
    int16_t *typeinfos;

    /* Return and argument types themselves. */
    MVMObject **types;

    /* The number of entries in typeinfos/types. */
    int32_t num_types;

    /* The MoarVM callsite object for this call. */
    MVMCallsite *cs;

#ifdef HAVE_LIBFFI
    ffi_abi     convention;
    ffi_type  **ffi_arg_types;
    ffi_type   *ffi_ret_type;
#endif

    /* The next entry in the linked list */
    MVMNativeCallback *next;
};


/* A hash of nativecall callbacks. Each entry is a linked
 * list of MVMNativeCallback sharing the same cuid.
 * Multiple callbacks with the same cuid get created when
 * closures are taken and need to be differentiated.
 */
struct MVMNativeCallbackCacheHead {
    struct MVMStrHashHandle hash_handle;
    MVMNativeCallback *head;
};

/* Functions for working with native callsites. */
MVMNativeCallBody * MVM_nativecall_get_nc_body(struct MVMThreadContext *tc, MVMObject *obj);
int16_t MVM_nativecall_get_arg_type(struct MVMThreadContext *tc, MVMObject *info, int16_t is_return);
int8_t MVM_nativecall_build(struct MVMThreadContext *tc, MVMObject *site, MVMString *lib,
    MVMString *sym, MVMString *conv, MVMObject *arg_spec, MVMObject *ret_spec);
void MVM_nativecall_setup(struct MVMThreadContext *tc, MVMNativeCallBody *body, unsigned int interval_id);
void MVM_nativecall_restore_library(struct MVMThreadContext *tc, MVMNativeCallBody *body, MVMObject *root);
MVMObject * MVM_nativecall_invoke(struct MVMThreadContext *tc, MVMObject *res_type,
    MVMObject *site, MVMObject *args);
void MVM_nativecall_dispatch(struct MVMThreadContext *tc, MVMObject *res_type,
    MVMObject *site, MVMArgs args);
MVMObject * MVM_nativecall_global(struct MVMThreadContext *tc, MVMString *lib, MVMString *sym,
    MVMObject *target_spec, MVMObject *target_type);
MVMObject * MVM_nativecall_cast(struct MVMThreadContext *tc, MVMObject *target_spec,
    MVMObject *res_type, MVMObject *obj);
int64_t MVM_nativecall_sizeof(struct MVMThreadContext *tc, MVMObject *obj);
void MVM_nativecall_refresh(struct MVMThreadContext *tc, MVMObject *cthingy);

MVMObject * MVM_nativecall_make_cstruct(struct MVMThreadContext *tc, MVMObject *type, void *cstruct);
MVMObject * MVM_nativecall_make_cppstruct(struct MVMThreadContext *tc, MVMObject *type, void *cppstruct);
MVMObject * MVM_nativecall_make_cunion(struct MVMThreadContext *tc, MVMObject *type, void *cunion);
MVMObject * MVM_nativecall_make_cpointer(struct MVMThreadContext *tc, MVMObject *type, void *ptr);
MVMObject * MVM_nativecall_make_carray(struct MVMThreadContext *tc, MVMObject *type, void *carray);

MVMObject * MVM_nativecall_make_int(struct MVMThreadContext *tc, MVMObject *type, int64_t value);
MVMObject * MVM_nativecall_make_uint(struct MVMThreadContext *tc, MVMObject *type, uint64_t value);
MVMObject * MVM_nativecall_make_num(struct MVMThreadContext *tc, MVMObject *type, double value);
MVMObject * MVM_nativecall_make_str(struct MVMThreadContext *tc, MVMObject *type, int16_t ret_type, char *cstring);

signed char         MVM_nativecall_unmarshal_char(struct MVMThreadContext *tc, MVMObject *value);
signed short        MVM_nativecall_unmarshal_short(struct MVMThreadContext *tc, MVMObject *value);
signed int          MVM_nativecall_unmarshal_int(struct MVMThreadContext *tc, MVMObject *value);
signed long         MVM_nativecall_unmarshal_long(struct MVMThreadContext *tc, MVMObject *value);
signed long long    MVM_nativecall_unmarshal_longlong(struct MVMThreadContext *tc, MVMObject *value);
unsigned char       MVM_nativecall_unmarshal_uchar(struct MVMThreadContext *tc, MVMObject *value);
unsigned short      MVM_nativecall_unmarshal_ushort(struct MVMThreadContext *tc, MVMObject *value);
unsigned int        MVM_nativecall_unmarshal_uint(struct MVMThreadContext *tc, MVMObject *value);
unsigned long       MVM_nativecall_unmarshal_ulong(struct MVMThreadContext *tc, MVMObject *value);
unsigned long long  MVM_nativecall_unmarshal_ulonglong(struct MVMThreadContext *tc, MVMObject *value);
float               MVM_nativecall_unmarshal_float(struct MVMThreadContext *tc, MVMObject *value);
double              MVM_nativecall_unmarshal_double(struct MVMThreadContext *tc, MVMObject *value);

char * MVM_nativecall_encode_string(struct MVMThreadContext *tc, MVMString *value_str, int16_t type, int16_t *free, int16_t unmarshal_kind, const MVMREPROps *repr);
char * MVM_nativecall_unmarshal_string(struct MVMThreadContext *tc, MVMObject *value, int16_t type, int16_t *free, int16_t unmarshal_kind);
void * MVM_nativecall_unmarshal_cstruct(struct MVMThreadContext *tc, MVMObject *value, int16_t unmarshal_kind);
void * MVM_nativecall_unmarshal_cppstruct(struct MVMThreadContext *tc, MVMObject *value, int16_t unmarshal_kind);
void * MVM_nativecall_unmarshal_cpointer(struct MVMThreadContext *tc, MVMObject *value, int16_t unmarshal_kind);
void * MVM_nativecall_unmarshal_carray(struct MVMThreadContext *tc, MVMObject *value, int16_t unmarshal_kind);
void * MVM_nativecall_unmarshal_vmarray(struct MVMThreadContext *tc, MVMObject *value, int16_t unmarshal_kind);
void * MVM_nativecall_unmarshal_cunion(struct MVMThreadContext *tc, MVMObject *value, int16_t unmarshal_kind);

/* Locate the thread that a callback should be run on. */
static inline struct MVMThreadContext * MVM_nativecall_find_thread_context(MVMInstance *instance) {
    struct MVMThreadContext *tc = MVM_get_running_threads_context();

    if (!tc)
        MVM_panic(1, "native callback ran on thread (%"PRId64") unknown to MoarVM",
                  MVM_platform_thread_id());

    return tc;
}


MVMJitGraph *MVM_nativecall_jit_graph_for_caller_code(
    struct MVMThreadContext   *tc,
    MVMSpeshGraph      *sg,
    MVMNativeCallBody  *body,
    int16_t            restype,
    int16_t            dst,
    MVMSpeshIns       **arg_ins
);
