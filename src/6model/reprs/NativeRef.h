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
const MVMREPROps * MVMNativeRef_initialize(MVMThreadContext *tc);

/* Operations on a nativeref REPR. */
void MVM_nativeref_ensure(MVMThreadContext *tc, MVMObject *val, uint16_t wantprim, uint16_t wantkind, char *guilty);
MVMObject * MVM_nativeref_lex_i(MVMThreadContext *tc, uint16_t outers, uint16_t idx);
MVMObject * MVM_nativeref_lex_u(MVMThreadContext *tc, uint16_t outers, uint16_t idx);
MVMObject * MVM_nativeref_lex_n(MVMThreadContext *tc, uint16_t outers, uint16_t idx);
MVMObject * MVM_nativeref_lex_s(MVMThreadContext *tc, uint16_t outers, uint16_t idx);
MVMObject * MVM_nativeref_lex_name_i(MVMThreadContext *tc, MVMString *name);
MVMObject * MVM_nativeref_lex_name_u(MVMThreadContext *tc, MVMString *name);
MVMObject * MVM_nativeref_lex_name_n(MVMThreadContext *tc, MVMString *name);
MVMObject * MVM_nativeref_lex_name_s(MVMThreadContext *tc, MVMString *name);
MVMObject * MVM_nativeref_attr_i(MVMThreadContext *tc, MVMObject *obj, MVMObject *class_handle, MVMString *name);
MVMObject * MVM_nativeref_attr_u(MVMThreadContext *tc, MVMObject *obj, MVMObject *class_handle, MVMString *name);
MVMObject * MVM_nativeref_attr_n(MVMThreadContext *tc, MVMObject *obj, MVMObject *class_handle, MVMString *name);
MVMObject * MVM_nativeref_attr_s(MVMThreadContext *tc, MVMObject *obj, MVMObject *class_handle, MVMString *name);
MVMObject * MVM_nativeref_pos_i(MVMThreadContext *tc, MVMObject *obj, int64_t idx);
MVMObject * MVM_nativeref_pos_u(MVMThreadContext *tc, MVMObject *obj, int64_t idx);
MVMObject * MVM_nativeref_pos_n(MVMThreadContext *tc, MVMObject *obj, int64_t idx);
MVMObject * MVM_nativeref_pos_s(MVMThreadContext *tc, MVMObject *obj, int64_t idx);
MVMObject * MVM_nativeref_multidim_i(MVMThreadContext *tc, MVMObject *obj, MVMObject *indices);
MVMObject * MVM_nativeref_multidim_u(MVMThreadContext *tc, MVMObject *obj, MVMObject *indices);
MVMObject * MVM_nativeref_multidim_n(MVMThreadContext *tc, MVMObject *obj, MVMObject *indices);
MVMObject * MVM_nativeref_multidim_s(MVMThreadContext *tc, MVMObject *obj, MVMObject *indices);
int64_t MVM_nativeref_read_lex_i(MVMThreadContext *tc, MVMObject *ref);
double MVM_nativeref_read_lex_n(MVMThreadContext *tc, MVMObject *ref);
MVMString * MVM_nativeref_read_lex_s(MVMThreadContext *tc, MVMObject *ref);
int64_t MVM_nativeref_read_attribute_i(MVMThreadContext *tc, MVMObject *ref);
uint64_t MVM_nativeref_read_attribute_u(MVMThreadContext *tc, MVMObject *ref);
double MVM_nativeref_read_attribute_n(MVMThreadContext *tc, MVMObject *ref);
MVMString * MVM_nativeref_read_attribute_s(MVMThreadContext *tc, MVMObject *ref);
int64_t MVM_nativeref_read_positional_i(MVMThreadContext *tc, MVMObject *ref);
uint64_t MVM_nativeref_read_positional_u(MVMThreadContext *tc, MVMObject *ref);
double MVM_nativeref_read_positional_n(MVMThreadContext *tc, MVMObject *ref);
MVMString * MVM_nativeref_read_positional_s(MVMThreadContext *tc, MVMObject *ref);
int64_t MVM_nativeref_read_multidim_i(MVMThreadContext *tc, MVMObject *ref);
uint64_t MVM_nativeref_read_multidim_u(MVMThreadContext *tc, MVMObject *ref);
double MVM_nativeref_read_multidim_n(MVMThreadContext *tc, MVMObject *ref);
MVMString * MVM_nativeref_read_multidim_s(MVMThreadContext *tc, MVMObject *ref);
void MVM_nativeref_write_lex_i(MVMThreadContext *tc, MVMObject *ref, int64_t value);
void MVM_nativeref_write_lex_u(MVMThreadContext *tc, MVMObject *ref, uint64_t value);
void MVM_nativeref_write_lex_n(MVMThreadContext *tc, MVMObject *ref, double value);
void MVM_nativeref_write_lex_s(MVMThreadContext *tc, MVMObject *ref, MVMString *value);
void MVM_nativeref_write_attribute_i(MVMThreadContext *tc, MVMObject *ref, int64_t value);
void MVM_nativeref_write_attribute_u(MVMThreadContext *tc, MVMObject *ref, uint64_t value);
void MVM_nativeref_write_attribute_n(MVMThreadContext *tc, MVMObject *ref, double value);
void MVM_nativeref_write_attribute_s(MVMThreadContext *tc, MVMObject *ref, MVMString *value);
void MVM_nativeref_write_positional_i(MVMThreadContext *tc, MVMObject *ref, int64_t value);
void MVM_nativeref_write_positional_u(MVMThreadContext *tc, MVMObject *ref, uint64_t value);
void MVM_nativeref_write_positional_n(MVMThreadContext *tc, MVMObject *ref, double value);
void MVM_nativeref_write_positional_s(MVMThreadContext *tc, MVMObject *ref, MVMString *value);
void MVM_nativeref_write_multidim_i(MVMThreadContext *tc, MVMObject *ref, int64_t value);
void MVM_nativeref_write_multidim_u(MVMThreadContext *tc, MVMObject *ref, uint64_t value);
void MVM_nativeref_write_multidim_n(MVMThreadContext *tc, MVMObject *ref, double value);
void MVM_nativeref_write_multidim_s(MVMThreadContext *tc, MVMObject *ref, MVMString *value);
AO_t * MVM_nativeref_as_atomic_lex_i(MVMThreadContext *tc, MVMObject *ref_obj);
AO_t * MVM_nativeref_as_atomic_attribute_i(MVMThreadContext *tc, MVMObject *ref_obj);
AO_t * MVM_nativeref_as_atomic_positional_i(MVMThreadContext *tc, MVMObject *ref_obj);
AO_t * MVM_nativeref_as_atomic_multidim_i(MVMThreadContext *tc, MVMObject *ref_obj);
