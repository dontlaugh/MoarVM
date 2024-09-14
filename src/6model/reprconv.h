void MVM_repr_init(MVMThreadContext *tc, MVMObject *obj);
MVM_PUBLIC MVMObject * MVM_repr_alloc(MVMThreadContext *tc, MVMObject *type);
MVM_PUBLIC MVMObject * MVM_repr_alloc_init(MVMThreadContext *tc, MVMObject *type);
MVM_PUBLIC MVMObject * MVM_repr_clone(MVMThreadContext *tc, MVMObject *obj);
void MVM_repr_compose(MVMThreadContext *tc, MVMObject *type, MVMObject *obj);

MVM_PUBLIC void MVM_repr_pos_set_elems(MVMThreadContext *tc, MVMObject *obj, int64_t elems);
void MVM_repr_populate_indices_array(MVMThreadContext *tc, MVMObject *arr, int64_t *elems);
MVM_PUBLIC void MVM_repr_set_dimensions(MVMThreadContext *tc, MVMObject *obj, MVMObject *dims);
MVM_PUBLIC MVMObject * MVM_repr_pos_slice(MVMThreadContext *tc, MVMObject *src, int64_t start, int64_t end);
MVM_PUBLIC void MVM_repr_pos_splice(MVMThreadContext *tc, MVMObject *obj, MVMObject *replacement, int64_t offset, int64_t count);

MVM_PUBLIC int64_t    MVM_repr_at_pos_i(MVMThreadContext *tc, MVMObject *obj, int64_t idx);
MVM_PUBLIC uint64_t   MVM_repr_at_pos_u(MVMThreadContext *tc, MVMObject *obj, int64_t idx);
MVM_PUBLIC double    MVM_repr_at_pos_n(MVMThreadContext *tc, MVMObject *obj, int64_t idx);
MVM_PUBLIC MVMString * MVM_repr_at_pos_s(MVMThreadContext *tc, MVMObject *obj, int64_t idx);
MVM_PUBLIC MVMObject * MVM_repr_at_pos_o(MVMThreadContext *tc, MVMObject *obj, int64_t idx);

MVM_PUBLIC int64_t    MVM_repr_at_pos_multidim_i(MVMThreadContext *tc, MVMObject *obj, MVMObject *indices);
MVM_PUBLIC uint64_t   MVM_repr_at_pos_multidim_u(MVMThreadContext *tc, MVMObject *obj, MVMObject *indices);
MVM_PUBLIC double    MVM_repr_at_pos_multidim_n(MVMThreadContext *tc, MVMObject *obj, MVMObject *indices);
MVM_PUBLIC MVMString * MVM_repr_at_pos_multidim_s(MVMThreadContext *tc, MVMObject *obj, MVMObject *indices);
MVM_PUBLIC MVMObject * MVM_repr_at_pos_multidim_o(MVMThreadContext *tc, MVMObject *obj, MVMObject *indices);

MVM_PUBLIC int64_t    MVM_repr_at_pos_2d_i(MVMThreadContext *tc, MVMObject *obj, int64_t idx1, int64_t idx2);
MVM_PUBLIC uint64_t   MVM_repr_at_pos_2d_u(MVMThreadContext *tc, MVMObject *obj, int64_t idx1, int64_t idx2);
MVM_PUBLIC double    MVM_repr_at_pos_2d_n(MVMThreadContext *tc, MVMObject *obj, int64_t idx1, int64_t idx2);
MVM_PUBLIC MVMString * MVM_repr_at_pos_2d_s(MVMThreadContext *tc, MVMObject *obj, int64_t idx1, int64_t idx2);
MVM_PUBLIC MVMObject * MVM_repr_at_pos_2d_o(MVMThreadContext *tc, MVMObject *obj, int64_t idx1, int64_t idx2);

MVM_PUBLIC int64_t    MVM_repr_at_pos_3d_i(MVMThreadContext *tc, MVMObject *obj, int64_t idx1, int64_t idx2, int64_t idx3);
MVM_PUBLIC uint64_t   MVM_repr_at_pos_3d_u(MVMThreadContext *tc, MVMObject *obj, int64_t idx1, int64_t idx2, int64_t idx3);
MVM_PUBLIC double    MVM_repr_at_pos_3d_n(MVMThreadContext *tc, MVMObject *obj, int64_t idx1, int64_t idx2, int64_t idx3);
MVM_PUBLIC MVMString * MVM_repr_at_pos_3d_s(MVMThreadContext *tc, MVMObject *obj, int64_t idx1, int64_t idx2, int64_t idx3);
MVM_PUBLIC MVMObject * MVM_repr_at_pos_3d_o(MVMThreadContext *tc, MVMObject *obj, int64_t idx1, int64_t idx2, int64_t idx3);

MVM_PUBLIC int64_t MVM_repr_exists_pos(MVMThreadContext *tc, MVMObject *obj, int64_t index);

MVM_PUBLIC void MVM_repr_bind_pos_i(MVMThreadContext *tc, MVMObject *obj, int64_t idx, int64_t value);
MVM_PUBLIC void MVM_repr_bind_pos_u(MVMThreadContext *tc, MVMObject *obj, int64_t idx, uint64_t value);
MVM_PUBLIC void MVM_repr_bind_pos_n(MVMThreadContext *tc, MVMObject *obj, int64_t idx, double value);
MVM_PUBLIC void MVM_repr_bind_pos_s(MVMThreadContext *tc, MVMObject *obj, int64_t idx, MVMString *value);
MVM_PUBLIC void MVM_repr_bind_pos_o(MVMThreadContext *tc, MVMObject *obj, int64_t idx, MVMObject *value);

MVM_PUBLIC void MVM_repr_write_buf(MVMThreadContext *tc, MVMObject *obj, char *value, int64_t offset, int64_t size);
MVM_PUBLIC int64_t MVM_repr_read_buf(MVMThreadContext *tc, MVMObject *obj, int64_t offset, int64_t size);

MVM_PUBLIC void MVM_repr_bind_pos_multidim_i(MVMThreadContext *tc, MVMObject *obj, MVMObject *indices, int64_t value);
MVM_PUBLIC void MVM_repr_bind_pos_multidim_u(MVMThreadContext *tc, MVMObject *obj, MVMObject *indices, uint64_t value);
MVM_PUBLIC void MVM_repr_bind_pos_multidim_n(MVMThreadContext *tc, MVMObject *obj, MVMObject *indices, double value);
MVM_PUBLIC void MVM_repr_bind_pos_multidim_s(MVMThreadContext *tc, MVMObject *obj, MVMObject *indices, MVMString *value);
MVM_PUBLIC void MVM_repr_bind_pos_multidim_o(MVMThreadContext *tc, MVMObject *obj, MVMObject *indices, MVMObject *value);

MVM_PUBLIC void MVM_repr_bind_pos_2d_i(MVMThreadContext *tc, MVMObject *obj, int64_t idx1, int64_t idx2, int64_t value);
MVM_PUBLIC void MVM_repr_bind_pos_2d_u(MVMThreadContext *tc, MVMObject *obj, int64_t idx1, int64_t idx2, uint64_t value);
MVM_PUBLIC void MVM_repr_bind_pos_2d_n(MVMThreadContext *tc, MVMObject *obj, int64_t idx1, int64_t idx2, double value);
MVM_PUBLIC void MVM_repr_bind_pos_2d_s(MVMThreadContext *tc, MVMObject *obj, int64_t idx1, int64_t idx2, MVMString *value);
MVM_PUBLIC void MVM_repr_bind_pos_2d_o(MVMThreadContext *tc, MVMObject *obj, int64_t idx1, int64_t idx2, MVMObject *value);

MVM_PUBLIC void MVM_repr_bind_pos_3d_i(MVMThreadContext *tc, MVMObject *obj, int64_t idx1, int64_t idx2, int64_t idx3, int64_t value);
MVM_PUBLIC void MVM_repr_bind_pos_3d_u(MVMThreadContext *tc, MVMObject *obj, int64_t idx1, int64_t idx2, int64_t idx3, uint64_t value);
MVM_PUBLIC void MVM_repr_bind_pos_3d_n(MVMThreadContext *tc, MVMObject *obj, int64_t idx1, int64_t idx2, int64_t idx3, double value);
MVM_PUBLIC void MVM_repr_bind_pos_3d_s(MVMThreadContext *tc, MVMObject *obj, int64_t idx1, int64_t idx2, int64_t idx3, MVMString *value);
MVM_PUBLIC void MVM_repr_bind_pos_3d_o(MVMThreadContext *tc, MVMObject *obj, int64_t idx1, int64_t idx2, int64_t idx3, MVMObject *value);

MVM_PUBLIC void MVM_repr_push_i(MVMThreadContext *tc, MVMObject *obj, int64_t pushee);
MVM_PUBLIC void MVM_repr_push_n(MVMThreadContext *tc, MVMObject *obj, double pushee);
MVM_PUBLIC void MVM_repr_push_s(MVMThreadContext *tc, MVMObject *obj, MVMString *pushee);
MVM_PUBLIC void MVM_repr_push_o(MVMThreadContext *tc, MVMObject *obj, MVMObject *pushee);

MVM_PUBLIC void MVM_repr_unshift_i(MVMThreadContext *tc, MVMObject *obj, int64_t unshiftee);
MVM_PUBLIC void MVM_repr_unshift_n(MVMThreadContext *tc, MVMObject *obj, double unshiftee);
MVM_PUBLIC void MVM_repr_unshift_s(MVMThreadContext *tc, MVMObject *obj, MVMString *unshiftee);
MVM_PUBLIC void MVM_repr_unshift_o(MVMThreadContext *tc, MVMObject *obj, MVMObject *unshiftee);

MVM_PUBLIC int64_t    MVM_repr_pop_i(MVMThreadContext *tc, MVMObject *obj);
MVM_PUBLIC double    MVM_repr_pop_n(MVMThreadContext *tc, MVMObject *obj);
MVM_PUBLIC MVMString * MVM_repr_pop_s(MVMThreadContext *tc, MVMObject *obj);
MVM_PUBLIC MVMObject * MVM_repr_pop_o(MVMThreadContext *tc, MVMObject *obj);

MVM_PUBLIC int64_t    MVM_repr_shift_i(MVMThreadContext *tc, MVMObject *obj);
MVM_PUBLIC double    MVM_repr_shift_n(MVMThreadContext *tc, MVMObject *obj);
MVM_PUBLIC MVMString * MVM_repr_shift_s(MVMThreadContext *tc, MVMObject *obj);
MVM_PUBLIC MVMObject * MVM_repr_shift_o(MVMThreadContext *tc, MVMObject *obj);

MVM_PUBLIC int64_t    MVM_repr_at_key_i(MVMThreadContext *tc, MVMObject *obj, MVMString *key);
MVM_PUBLIC uint64_t   MVM_repr_at_key_u(MVMThreadContext *tc, MVMObject *obj, MVMString *key);
MVM_PUBLIC double    MVM_repr_at_key_n(MVMThreadContext *tc, MVMObject *obj, MVMString *key);
MVM_PUBLIC MVMString * MVM_repr_at_key_s(MVMThreadContext *tc, MVMObject *obj, MVMString *key);
MVM_PUBLIC MVMObject * MVM_repr_at_key_o(MVMThreadContext *tc, MVMObject *obj, MVMString *key);

MVM_PUBLIC void MVM_repr_bind_key_i(MVMThreadContext *tc, MVMObject *obj, MVMString *key, int64_t val);
MVM_PUBLIC void MVM_repr_bind_key_n(MVMThreadContext *tc, MVMObject *obj, MVMString *key, double val);
MVM_PUBLIC void MVM_repr_bind_key_s(MVMThreadContext *tc, MVMObject *obj, MVMString *key, MVMString *val);
MVM_PUBLIC void MVM_repr_bind_key_o(MVMThreadContext *tc, MVMObject *obj, MVMString *key, MVMObject *val);

MVM_PUBLIC int64_t MVM_repr_exists_key(MVMThreadContext *tc, MVMObject *obj, MVMString *key);
MVM_PUBLIC void MVM_repr_delete_key(MVMThreadContext *tc, MVMObject *obj, MVMString *key);

MVM_PUBLIC uint64_t MVM_repr_elems(MVMThreadContext *tc, MVMObject *obj);
MVM_PUBLIC MVMObject * MVM_repr_dimensions(MVMThreadContext *tc, MVMObject *obj);
MVM_PUBLIC int64_t MVM_repr_num_dimensions(MVMThreadContext *tc, MVMObject *obj);

MVM_PUBLIC int64_t MVM_repr_get_int(MVMThreadContext *tc, MVMObject *obj);
MVM_PUBLIC double MVM_repr_get_num(MVMThreadContext *tc, MVMObject *obj);
MVM_PUBLIC MVMString * MVM_repr_get_str(MVMThreadContext *tc, MVMObject *obj);
MVM_PUBLIC uint64_t MVM_repr_get_uint(MVMThreadContext *tc, MVMObject *obj);
MVM_PUBLIC void MVM_repr_set_int(MVMThreadContext *tc, MVMObject *obj, int64_t val);
MVM_PUBLIC void MVM_repr_set_num(MVMThreadContext *tc, MVMObject *obj, double val);
MVM_PUBLIC void MVM_repr_set_str(MVMThreadContext *tc, MVMObject *obj, MVMString *val);
MVM_PUBLIC void MVM_repr_set_uint(MVMThreadContext *tc, MVMObject *obj, uint64_t val);

MVM_PUBLIC MVMObject * MVM_repr_box_int(MVMThreadContext *tc, MVMObject *type, int64_t val);
MVM_PUBLIC MVMObject * MVM_repr_box_num(MVMThreadContext *tc, MVMObject *type, double val);
MVM_PUBLIC MVMObject * MVM_repr_box_str(MVMThreadContext *tc, MVMObject *type, MVMString *val);
MVM_PUBLIC MVMObject * MVM_repr_box_uint(MVMThreadContext *tc, MVMObject *type, uint64_t val);

MVM_PUBLIC int64_t    MVM_repr_get_attr_i(MVMThreadContext *tc, MVMObject *object, MVMObject *type,
                                           MVMString *name, int16_t hint);
MVM_PUBLIC uint64_t   MVM_repr_get_attr_u(MVMThreadContext *tc, MVMObject *object, MVMObject *type,
                                           MVMString *name, int16_t hint);
MVM_PUBLIC double    MVM_repr_get_attr_n(MVMThreadContext *tc, MVMObject *object, MVMObject *type,
                                           MVMString *name, int16_t hint);
MVM_PUBLIC MVMString * MVM_repr_get_attr_s(MVMThreadContext *tc, MVMObject *object, MVMObject *type,
                                           MVMString *name, int16_t hint);
MVM_PUBLIC MVMObject * MVM_repr_get_attr_o(MVMThreadContext *tc, MVMObject *object, MVMObject *type,
                                           MVMString *name, int16_t hint);

MVM_PUBLIC void        MVM_repr_bind_attr_inso(MVMThreadContext *tc, MVMObject *object, MVMObject *type,
                                               MVMString *name, int16_t hint, MVMRegister value_reg, uint16_t kind);

MVM_PUBLIC int64_t   MVM_repr_attribute_inited(MVMThreadContext *tc, MVMObject *object, MVMObject *type,
                                                MVMString *name);

MVM_PUBLIC int64_t    MVM_repr_compare_repr_id(MVMThreadContext *tc, MVMObject *object, uint32_t REPRId);

MVM_PUBLIC int64_t    MVM_repr_hint_for(MVMThreadContext *tc, MVMObject *object, MVMString *attrname);

MVM_PUBLIC void MVM_repr_atomic_bind_attr_o(MVMThreadContext *tc, MVMObject *object,
                                            MVMObject *type, MVMString *name,
                                            MVMObject *value);

MVM_PUBLIC MVMObject * MVM_repr_casattr_o(MVMThreadContext *tc, MVMObject *object,
                                          MVMObject *type, MVMString *name,
                                          MVMObject *expected, MVMObject *value);

#define MVM_repr_at_key_int(tc, obj, key) \
    MVM_repr_get_int((tc), MVM_repr_at_key_o((tc), (obj), (key)))
#define MVM_repr_at_key_num(tc, obj, key) \
    MVM_repr_get_num((tc), MVM_repr_at_key_o((tc), (obj), (key)))
#define MVM_repr_at_key_str(tc, obj, key) \
    MVM_repr_get_str((tc), MVM_repr_at_key_o((tc), (obj), (key)))

#define MVM_repr_bind_key_int(tc, obj, key, val) do { \
    MVMObject *boxed = MVM_repr_box_int((tc), (*((tc)->interp_cu))->body.hll_config->int_box_type, (val)); \
    MVM_repr_bind_key_o((tc), (obj), (key), boxed); \
} while (0)

#define MVM_repr_bind_key_num(tc, obj, key, val) do {\
    MVMObject *boxed = MVM_repr_box_int((tc), (*((tc)->interp_cu))->body.hll_config->num_box_type, (val)); \
    MVM_repr_bind_key_o((tc), (obj), (key), boxed); \
} while (0)

#define MVM_repr_bind_key_str(tc, obj, key, val) do {\
    MVMObject *boxed = MVM_repr_box_int((tc), (*((tc)->interp_cu))->body.hll_config->str_box_type, (val)); \
    MVM_repr_bind_key_o((tc), (obj), (key), boxed); \
} while (0)
