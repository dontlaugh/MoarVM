void MVM_repr_init(struct MVMThreadContext *tc,struct MVMObject *obj);
struct MVMObject * MVM_repr_alloc(struct MVMThreadContext *tc,struct MVMObject *type);
struct MVMObject * MVM_repr_alloc_init(struct MVMThreadContext *tc,struct MVMObject *type);
struct MVMObject * MVM_repr_clone(struct MVMThreadContext *tc,struct MVMObject *obj);
void MVM_repr_compose(struct MVMThreadContext *tc,struct MVMObject *type,struct MVMObject *obj);

 void MVM_repr_pos_set_elems(struct MVMThreadContext *tc,struct MVMObject *obj, int64_t elems);
void MVM_repr_populate_indices_array(struct MVMThreadContext *tc,struct MVMObject *arr, int64_t *elems);
 void MVM_repr_set_dimensions(struct MVMThreadContext *tc,struct MVMObject *obj,struct MVMObject *dims);
struct MVMObject * MVM_repr_pos_slice(struct MVMThreadContext *tc,struct MVMObject *src, int64_t start, int64_t end);
 void MVM_repr_pos_splice(struct MVMThreadContext *tc,struct MVMObject *obj,struct MVMObject *replacement, int64_t offset, int64_t count);

 int64_t    MVM_repr_at_pos_i(struct MVMThreadContext *tc,struct MVMObject *obj, int64_t idx);
 uint64_t   MVM_repr_at_pos_u(struct MVMThreadContext *tc,struct MVMObject *obj, int64_t idx);
 double    MVM_repr_at_pos_n(struct MVMThreadContext *tc,struct MVMObject *obj, int64_t idx);
 MVMString * MVM_repr_at_pos_s(struct MVMThreadContext *tc,struct MVMObject *obj, int64_t idx);
struct MVMObject * MVM_repr_at_pos_o(struct MVMThreadContext *tc,struct MVMObject *obj, int64_t idx);

 int64_t    MVM_repr_at_pos_multidim_i(struct MVMThreadContext *tc,struct MVMObject *obj,struct MVMObject *indices);
 uint64_t   MVM_repr_at_pos_multidim_u(struct MVMThreadContext *tc,struct MVMObject *obj,struct MVMObject *indices);
 double    MVM_repr_at_pos_multidim_n(struct MVMThreadContext *tc,struct MVMObject *obj,struct MVMObject *indices);
 MVMString * MVM_repr_at_pos_multidim_s(struct MVMThreadContext *tc,struct MVMObject *obj,struct MVMObject *indices);
struct MVMObject * MVM_repr_at_pos_multidim_o(struct MVMThreadContext *tc,struct MVMObject *obj,struct MVMObject *indices);

 int64_t    MVM_repr_at_pos_2d_i(struct MVMThreadContext *tc,struct MVMObject *obj, int64_t idx1, int64_t idx2);
 uint64_t   MVM_repr_at_pos_2d_u(struct MVMThreadContext *tc,struct MVMObject *obj, int64_t idx1, int64_t idx2);
 double    MVM_repr_at_pos_2d_n(struct MVMThreadContext *tc,struct MVMObject *obj, int64_t idx1, int64_t idx2);
 MVMString * MVM_repr_at_pos_2d_s(struct MVMThreadContext *tc,struct MVMObject *obj, int64_t idx1, int64_t idx2);
struct MVMObject * MVM_repr_at_pos_2d_o(struct MVMThreadContext *tc,struct MVMObject *obj, int64_t idx1, int64_t idx2);

 int64_t    MVM_repr_at_pos_3d_i(struct MVMThreadContext *tc,struct MVMObject *obj, int64_t idx1, int64_t idx2, int64_t idx3);
 uint64_t   MVM_repr_at_pos_3d_u(struct MVMThreadContext *tc,struct MVMObject *obj, int64_t idx1, int64_t idx2, int64_t idx3);
 double    MVM_repr_at_pos_3d_n(struct MVMThreadContext *tc,struct MVMObject *obj, int64_t idx1, int64_t idx2, int64_t idx3);
 MVMString * MVM_repr_at_pos_3d_s(struct MVMThreadContext *tc,struct MVMObject *obj, int64_t idx1, int64_t idx2, int64_t idx3);
struct MVMObject * MVM_repr_at_pos_3d_o(struct MVMThreadContext *tc,struct MVMObject *obj, int64_t idx1, int64_t idx2, int64_t idx3);

 int64_t MVM_repr_exists_pos(struct MVMThreadContext *tc,struct MVMObject *obj, int64_t index);

 void MVM_repr_bind_pos_i(struct MVMThreadContext *tc,struct MVMObject *obj, int64_t idx, int64_t value);
 void MVM_repr_bind_pos_u(struct MVMThreadContext *tc,struct MVMObject *obj, int64_t idx, uint64_t value);
 void MVM_repr_bind_pos_n(struct MVMThreadContext *tc,struct MVMObject *obj, int64_t idx, double value);
 void MVM_repr_bind_pos_s(struct MVMThreadContext *tc,struct MVMObject *obj, int64_t idx, MVMString *value);
 void MVM_repr_bind_pos_o(struct MVMThreadContext *tc,struct MVMObject *obj, int64_t idx,struct MVMObject *value);

 void MVM_repr_write_buf(struct MVMThreadContext *tc,struct MVMObject *obj, char *value, int64_t offset, int64_t size);
 int64_t MVM_repr_read_buf(struct MVMThreadContext *tc,struct MVMObject *obj, int64_t offset, int64_t size);

 void MVM_repr_bind_pos_multidim_i(struct MVMThreadContext *tc,struct MVMObject *obj,struct MVMObject *indices, int64_t value);
 void MVM_repr_bind_pos_multidim_u(struct MVMThreadContext *tc,struct MVMObject *obj,struct MVMObject *indices, uint64_t value);
 void MVM_repr_bind_pos_multidim_n(struct MVMThreadContext *tc,struct MVMObject *obj,struct MVMObject *indices, double value);
 void MVM_repr_bind_pos_multidim_s(struct MVMThreadContext *tc,struct MVMObject *obj,struct MVMObject *indices, MVMString *value);
 void MVM_repr_bind_pos_multidim_o(struct MVMThreadContext *tc,struct MVMObject *obj,struct MVMObject *indices,struct MVMObject *value);

 void MVM_repr_bind_pos_2d_i(struct MVMThreadContext *tc,struct MVMObject *obj, int64_t idx1, int64_t idx2, int64_t value);
 void MVM_repr_bind_pos_2d_u(struct MVMThreadContext *tc,struct MVMObject *obj, int64_t idx1, int64_t idx2, uint64_t value);
 void MVM_repr_bind_pos_2d_n(struct MVMThreadContext *tc,struct MVMObject *obj, int64_t idx1, int64_t idx2, double value);
 void MVM_repr_bind_pos_2d_s(struct MVMThreadContext *tc,struct MVMObject *obj, int64_t idx1, int64_t idx2, MVMString *value);
 void MVM_repr_bind_pos_2d_o(struct MVMThreadContext *tc,struct MVMObject *obj, int64_t idx1, int64_t idx2,struct MVMObject *value);

 void MVM_repr_bind_pos_3d_i(struct MVMThreadContext *tc,struct MVMObject *obj, int64_t idx1, int64_t idx2, int64_t idx3, int64_t value);
 void MVM_repr_bind_pos_3d_u(struct MVMThreadContext *tc,struct MVMObject *obj, int64_t idx1, int64_t idx2, int64_t idx3, uint64_t value);
 void MVM_repr_bind_pos_3d_n(struct MVMThreadContext *tc,struct MVMObject *obj, int64_t idx1, int64_t idx2, int64_t idx3, double value);
 void MVM_repr_bind_pos_3d_s(struct MVMThreadContext *tc,struct MVMObject *obj, int64_t idx1, int64_t idx2, int64_t idx3, MVMString *value);
 void MVM_repr_bind_pos_3d_o(struct MVMThreadContext *tc,struct MVMObject *obj, int64_t idx1, int64_t idx2, int64_t idx3,struct MVMObject *value);

 void MVM_repr_push_i(struct MVMThreadContext *tc,struct MVMObject *obj, int64_t pushee);
 void MVM_repr_push_n(struct MVMThreadContext *tc,struct MVMObject *obj, double pushee);
 void MVM_repr_push_s(struct MVMThreadContext *tc,struct MVMObject *obj, MVMString *pushee);
 void MVM_repr_push_o(struct MVMThreadContext *tc,struct MVMObject *obj,struct MVMObject *pushee);

 void MVM_repr_unshift_i(struct MVMThreadContext *tc,struct MVMObject *obj, int64_t unshiftee);
 void MVM_repr_unshift_n(struct MVMThreadContext *tc,struct MVMObject *obj, double unshiftee);
 void MVM_repr_unshift_s(struct MVMThreadContext *tc,struct MVMObject *obj, MVMString *unshiftee);
 void MVM_repr_unshift_o(struct MVMThreadContext *tc,struct MVMObject *obj,struct MVMObject *unshiftee);

 int64_t    MVM_repr_pop_i(struct MVMThreadContext *tc,struct MVMObject *obj);
 double    MVM_repr_pop_n(struct MVMThreadContext *tc,struct MVMObject *obj);
 MVMString * MVM_repr_pop_s(struct MVMThreadContext *tc,struct MVMObject *obj);
struct MVMObject * MVM_repr_pop_o(struct MVMThreadContext *tc,struct MVMObject *obj);

 int64_t    MVM_repr_shift_i(struct MVMThreadContext *tc,struct MVMObject *obj);
 double    MVM_repr_shift_n(struct MVMThreadContext *tc,struct MVMObject *obj);
 MVMString * MVM_repr_shift_s(struct MVMThreadContext *tc,struct MVMObject *obj);
struct MVMObject * MVM_repr_shift_o(struct MVMThreadContext *tc,struct MVMObject *obj);

 int64_t    MVM_repr_at_key_i(struct MVMThreadContext *tc,struct MVMObject *obj, MVMString *key);
 uint64_t   MVM_repr_at_key_u(struct MVMThreadContext *tc,struct MVMObject *obj, MVMString *key);
 double    MVM_repr_at_key_n(struct MVMThreadContext *tc,struct MVMObject *obj, MVMString *key);
 MVMString * MVM_repr_at_key_s(struct MVMThreadContext *tc,struct MVMObject *obj, MVMString *key);
struct MVMObject * MVM_repr_at_key_o(struct MVMThreadContext *tc,struct MVMObject *obj, MVMString *key);

 void MVM_repr_bind_key_i(struct MVMThreadContext *tc,struct MVMObject *obj, MVMString *key, int64_t val);
 void MVM_repr_bind_key_n(struct MVMThreadContext *tc,struct MVMObject *obj, MVMString *key, double val);
 void MVM_repr_bind_key_s(struct MVMThreadContext *tc,struct MVMObject *obj, MVMString *key, MVMString *val);
 void MVM_repr_bind_key_o(struct MVMThreadContext *tc,struct MVMObject *obj, MVMString *key,struct MVMObject *val);

 int64_t MVM_repr_exists_key(struct MVMThreadContext *tc,struct MVMObject *obj, MVMString *key);
 void MVM_repr_delete_key(struct MVMThreadContext *tc,struct MVMObject *obj, MVMString *key);

 uint64_t MVM_repr_elems(struct MVMThreadContext *tc,struct MVMObject *obj);
struct MVMObject * MVM_repr_dimensions(struct MVMThreadContext *tc,struct MVMObject *obj);
 int64_t MVM_repr_num_dimensions(struct MVMThreadContext *tc,struct MVMObject *obj);

 int64_t MVM_repr_get_int(struct MVMThreadContext *tc,struct MVMObject *obj);
 double MVM_repr_get_num(struct MVMThreadContext *tc,struct MVMObject *obj);
 MVMString * MVM_repr_get_str(struct MVMThreadContext *tc,struct MVMObject *obj);
 uint64_t MVM_repr_get_uint(struct MVMThreadContext *tc,struct MVMObject *obj);
 void MVM_repr_set_int(struct MVMThreadContext *tc,struct MVMObject *obj, int64_t val);
 void MVM_repr_set_num(struct MVMThreadContext *tc,struct MVMObject *obj, double val);
 void MVM_repr_set_str(struct MVMThreadContext *tc,struct MVMObject *obj, MVMString *val);
 void MVM_repr_set_uint(struct MVMThreadContext *tc,struct MVMObject *obj, uint64_t val);

struct MVMObject * MVM_repr_box_int(struct MVMThreadContext *tc,struct MVMObject *type, int64_t val);
struct MVMObject * MVM_repr_box_num(struct MVMThreadContext *tc,struct MVMObject *type, double val);
struct MVMObject * MVM_repr_box_str(struct MVMThreadContext *tc,struct MVMObject *type, MVMString *val);
struct MVMObject * MVM_repr_box_uint(struct MVMThreadContext *tc,struct MVMObject *type, uint64_t val);

 int64_t    MVM_repr_get_attr_i(struct MVMThreadContext *tc,struct MVMObject *object,struct MVMObject *type,
                                           MVMString *name, int16_t hint);
 uint64_t   MVM_repr_get_attr_u(struct MVMThreadContext *tc,struct MVMObject *object,struct MVMObject *type,
                                           MVMString *name, int16_t hint);
 double    MVM_repr_get_attr_n(struct MVMThreadContext *tc,struct MVMObject *object,struct MVMObject *type,
                                           MVMString *name, int16_t hint);
 MVMString * MVM_repr_get_attr_s(struct MVMThreadContext *tc,struct MVMObject *object,struct MVMObject *type,
                                           MVMString *name, int16_t hint);
struct MVMObject * MVM_repr_get_attr_o(struct MVMThreadContext *tc,struct MVMObject *object,struct MVMObject *type,
                                           MVMString *name, int16_t hint);

 void        MVM_repr_bind_attr_inso(struct MVMThreadContext *tc,struct MVMObject *object,struct MVMObject *type,
                                               MVMString *name, int16_t hint, MVMRegister value_reg, uint16_t kind);

 int64_t   MVM_repr_attribute_inited(struct MVMThreadContext *tc,struct MVMObject *object,struct MVMObject *type,
                                                MVMString *name);

 int64_t    MVM_repr_compare_repr_id(struct MVMThreadContext *tc,struct MVMObject *object, uint32_t REPRId);

 int64_t    MVM_repr_hint_for(struct MVMThreadContext *tc,struct MVMObject *object, MVMString *attrname);

 void MVM_repr_atomic_bind_attr_o(struct MVMThreadContext *tc,struct MVMObject *object,
                                           struct MVMObject *type, MVMString *name,
                                           struct MVMObject *value);

struct MVMObject * MVM_repr_casattr_o(struct MVMThreadContext *tc,struct MVMObject *object,
                                         struct MVMObject *type, MVMString *name,
                                         struct MVMObject *expected,struct MVMObject *value);

#define MVM_repr_at_key_int(tc, obj, key) \
    MVM_repr_get_int((tc), MVM_repr_at_key_o((tc), (obj), (key)))
#define MVM_repr_at_key_num(tc, obj, key) \
    MVM_repr_get_num((tc), MVM_repr_at_key_o((tc), (obj), (key)))
#define MVM_repr_at_key_str(tc, obj, key) \
    MVM_repr_get_str((tc), MVM_repr_at_key_o((tc), (obj), (key)))

#define MVM_repr_bind_key_int(tc, obj, key, val) do { \
   struct MVMObject *boxed = MVM_repr_box_int((tc), (*((tc)->interp_cu))->body.hll_config->int_box_type, (val)); \
    MVM_repr_bind_key_o((tc), (obj), (key), boxed); \
} while (0)

#define MVM_repr_bind_key_num(tc, obj, key, val) do {\
   struct MVMObject *boxed = MVM_repr_box_int((tc), (*((tc)->interp_cu))->body.hll_config->num_box_type, (val)); \
    MVM_repr_bind_key_o((tc), (obj), (key), boxed); \
} while (0)

#define MVM_repr_bind_key_str(tc, obj, key, val) do {\
   struct MVMObject *boxed = MVM_repr_box_int((tc), (*((tc)->interp_cu))->body.hll_config->str_box_type, (val)); \
    MVM_repr_bind_key_o((tc), (obj), (key), boxed); \
} while (0)
