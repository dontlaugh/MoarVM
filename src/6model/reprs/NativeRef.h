/* Representation of a reference to a native value. */
struct MVMNativeRefBody {
    union {
        struct {
            MVMFrame *frame;
            uint16_t env_idx;
            uint16_t type;
        } lex;
        struct {
            MVMObject *obj;
            MVMObject *class_handle;
            MVMString *name;
        } attribute;
        struct {
            MVMObject *obj;
            int64_t idx;
        } positional;
        struct {
            MVMObject *obj;
            MVMObject *indices;
        } multidim;
    } u;
};
struct MVMNativeRef {
    MVMObject common;
    MVMNativeRefBody body;
};

/* Kinds of native reference. */
#define MVM_NATIVEREF_LEX           1
#define MVM_NATIVEREF_ATTRIBUTE     2
#define MVM_NATIVEREF_POSITIONAL    3
#define MVM_NATIVEREF_MULTIDIM      4

/* REPR data for a native reference. */
struct MVMNativeRefREPRData {
    /* The primitive type of native reference this is (one of the values that
     * is valid for MVMStorageSpec.boxed_primitive). */
    uint16_t primitive_type;

    /* The kind of reference this is. */
    uint16_t ref_kind;

    uint8_t is_unsigned;
};

/* Function for REPR setup. */
const MVMREPROps * MVMNativeRef_initialize(struct MVMThreadContext *tc);

/* Operations on a nativeref REPR. */
void MVM_nativeref_ensure(struct MVMThreadContext *tc, MVMObject *val, uint16_t wantprim, uint16_t wantkind, char *guilty);
MVMObject * MVM_nativeref_lex_i(struct MVMThreadContext *tc, uint16_t outers, uint16_t idx);
MVMObject * MVM_nativeref_lex_u(struct MVMThreadContext *tc, uint16_t outers, uint16_t idx);
MVMObject * MVM_nativeref_lex_n(struct MVMThreadContext *tc, uint16_t outers, uint16_t idx);
MVMObject * MVM_nativeref_lex_s(struct MVMThreadContext *tc, uint16_t outers, uint16_t idx);
MVMObject * MVM_nativeref_lex_name_i(struct MVMThreadContext *tc, MVMString *name);
MVMObject * MVM_nativeref_lex_name_u(struct MVMThreadContext *tc, MVMString *name);
MVMObject * MVM_nativeref_lex_name_n(struct MVMThreadContext *tc, MVMString *name);
MVMObject * MVM_nativeref_lex_name_s(struct MVMThreadContext *tc, MVMString *name);
MVMObject * MVM_nativeref_attr_i(struct MVMThreadContext *tc, MVMObject *obj, MVMObject *class_handle, MVMString *name);
MVMObject * MVM_nativeref_attr_u(struct MVMThreadContext *tc, MVMObject *obj, MVMObject *class_handle, MVMString *name);
MVMObject * MVM_nativeref_attr_n(struct MVMThreadContext *tc, MVMObject *obj, MVMObject *class_handle, MVMString *name);
MVMObject * MVM_nativeref_attr_s(struct MVMThreadContext *tc, MVMObject *obj, MVMObject *class_handle, MVMString *name);
MVMObject * MVM_nativeref_pos_i(struct MVMThreadContext *tc, MVMObject *obj, int64_t idx);
MVMObject * MVM_nativeref_pos_u(struct MVMThreadContext *tc, MVMObject *obj, int64_t idx);
MVMObject * MVM_nativeref_pos_n(struct MVMThreadContext *tc, MVMObject *obj, int64_t idx);
MVMObject * MVM_nativeref_pos_s(struct MVMThreadContext *tc, MVMObject *obj, int64_t idx);
MVMObject * MVM_nativeref_multidim_i(struct MVMThreadContext *tc, MVMObject *obj, MVMObject *indices);
MVMObject * MVM_nativeref_multidim_u(struct MVMThreadContext *tc, MVMObject *obj, MVMObject *indices);
MVMObject * MVM_nativeref_multidim_n(struct MVMThreadContext *tc, MVMObject *obj, MVMObject *indices);
MVMObject * MVM_nativeref_multidim_s(struct MVMThreadContext *tc, MVMObject *obj, MVMObject *indices);
int64_t MVM_nativeref_read_lex_i(struct MVMThreadContext *tc, MVMObject *ref);
double MVM_nativeref_read_lex_n(struct MVMThreadContext *tc, MVMObject *ref);
MVMString * MVM_nativeref_read_lex_s(struct MVMThreadContext *tc, MVMObject *ref);
int64_t MVM_nativeref_read_attribute_i(struct MVMThreadContext *tc, MVMObject *ref);
uint64_t MVM_nativeref_read_attribute_u(struct MVMThreadContext *tc, MVMObject *ref);
double MVM_nativeref_read_attribute_n(struct MVMThreadContext *tc, MVMObject *ref);
MVMString * MVM_nativeref_read_attribute_s(struct MVMThreadContext *tc, MVMObject *ref);
int64_t MVM_nativeref_read_positional_i(struct MVMThreadContext *tc, MVMObject *ref);
uint64_t MVM_nativeref_read_positional_u(struct MVMThreadContext *tc, MVMObject *ref);
double MVM_nativeref_read_positional_n(struct MVMThreadContext *tc, MVMObject *ref);
MVMString * MVM_nativeref_read_positional_s(struct MVMThreadContext *tc, MVMObject *ref);
int64_t MVM_nativeref_read_multidim_i(struct MVMThreadContext *tc, MVMObject *ref);
uint64_t MVM_nativeref_read_multidim_u(struct MVMThreadContext *tc, MVMObject *ref);
double MVM_nativeref_read_multidim_n(struct MVMThreadContext *tc, MVMObject *ref);
MVMString * MVM_nativeref_read_multidim_s(struct MVMThreadContext *tc, MVMObject *ref);
void MVM_nativeref_write_lex_i(struct MVMThreadContext *tc, MVMObject *ref, int64_t value);
void MVM_nativeref_write_lex_u(struct MVMThreadContext *tc, MVMObject *ref, uint64_t value);
void MVM_nativeref_write_lex_n(struct MVMThreadContext *tc, MVMObject *ref, double value);
void MVM_nativeref_write_lex_s(struct MVMThreadContext *tc, MVMObject *ref, MVMString *value);
void MVM_nativeref_write_attribute_i(struct MVMThreadContext *tc, MVMObject *ref, int64_t value);
void MVM_nativeref_write_attribute_u(struct MVMThreadContext *tc, MVMObject *ref, uint64_t value);
void MVM_nativeref_write_attribute_n(struct MVMThreadContext *tc, MVMObject *ref, double value);
void MVM_nativeref_write_attribute_s(struct MVMThreadContext *tc, MVMObject *ref, MVMString *value);
void MVM_nativeref_write_positional_i(struct MVMThreadContext *tc, MVMObject *ref, int64_t value);
void MVM_nativeref_write_positional_u(struct MVMThreadContext *tc, MVMObject *ref, uint64_t value);
void MVM_nativeref_write_positional_n(struct MVMThreadContext *tc, MVMObject *ref, double value);
void MVM_nativeref_write_positional_s(struct MVMThreadContext *tc, MVMObject *ref, MVMString *value);
void MVM_nativeref_write_multidim_i(struct MVMThreadContext *tc, MVMObject *ref, int64_t value);
void MVM_nativeref_write_multidim_u(struct MVMThreadContext *tc, MVMObject *ref, uint64_t value);
void MVM_nativeref_write_multidim_n(struct MVMThreadContext *tc, MVMObject *ref, double value);
void MVM_nativeref_write_multidim_s(struct MVMThreadContext *tc, MVMObject *ref, MVMString *value);
atomic_uintptr_t * MVM_nativeref_as_atomic_lex_i(struct MVMThreadContext *tc, MVMObject *ref_obj);
atomic_uintptr_t * MVM_nativeref_as_atomic_attribute_i(struct MVMThreadContext *tc, MVMObject *ref_obj);
atomic_uintptr_t * MVM_nativeref_as_atomic_positional_i(struct MVMThreadContext *tc, MVMObject *ref_obj);
atomic_uintptr_t * MVM_nativeref_as_atomic_multidim_i(struct MVMThreadContext *tc, MVMObject *ref_obj);
