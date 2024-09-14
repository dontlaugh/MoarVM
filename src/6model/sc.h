/* SC manipulation functions. */
MVMObject * MVM_sc_create(struct MVMThreadContext *tc, MVMString *handle);
void MVM_sc_add_all_scs_entry(struct MVMThreadContext *tc, MVMSerializationContextBody *scb);
void MVM_sc_all_scs_destroy(struct MVMThreadContext *tc);
MVMString * MVM_sc_get_handle(struct MVMThreadContext *tc, MVMSerializationContext *sc);
MVMString * MVM_sc_get_description(struct MVMThreadContext *tc, MVMSerializationContext *sc);
void MVM_sc_set_description(struct MVMThreadContext *tc, MVMSerializationContext *sc, MVMString *desc);
int64_t MVM_sc_find_object_idx(struct MVMThreadContext *tc, MVMSerializationContext *sc, MVMObject *obj);
int64_t MVM_sc_find_object_idx_jit(struct MVMThreadContext *tc, MVMObject *sc, MVMObject *obj);
int64_t MVM_sc_find_stable_idx(struct MVMThreadContext *tc, MVMSerializationContext *sc, MVMSTable *st);
int64_t MVM_sc_find_code_idx(struct MVMThreadContext *tc, MVMSerializationContext *sc, MVMObject *obj);
uint8_t MVM_sc_is_object_immediately_available(struct MVMThreadContext *tc, MVMSerializationContext *sc, int64_t idx);
MVMObject * MVM_sc_get_object(struct MVMThreadContext *tc, MVMSerializationContext *sc, int64_t idx);
MVMObject * MVM_sc_try_get_object(struct MVMThreadContext *tc, MVMSerializationContext *sc, int64_t idx);
void MVM_sc_set_object(struct MVMThreadContext *tc, MVMSerializationContext *sc, int64_t idx, MVMObject *obj);
void MVM_sc_set_object_op(struct MVMThreadContext *tc, MVMSerializationContext *sc, int64_t idx, MVMObject *obj);
void MVM_sc_set_object_no_update(struct MVMThreadContext *tc, MVMSerializationContext *sc, int64_t idx, MVMObject *obj);
MVMSTable * MVM_sc_get_stable(struct MVMThreadContext *tc, MVMSerializationContext *sc, int64_t idx);
MVMSTable * MVM_sc_try_get_stable(struct MVMThreadContext *tc, MVMSerializationContext *sc, int64_t idx);
void MVM_sc_set_stable(struct MVMThreadContext *tc, MVMSerializationContext *sc, int64_t idx, MVMSTable *st);
void MVM_sc_push_stable(struct MVMThreadContext *tc, MVMSerializationContext *sc, MVMSTable *st);
MVMObject * MVM_sc_get_code(struct MVMThreadContext *tc, MVMSerializationContext *sc, int64_t idx);
void MVM_sc_set_code_op(struct MVMThreadContext *tc, MVMSerializationContext *sc, int64_t idx, MVMObject *code);
MVMSerializationContext * MVM_sc_find_by_handle(struct MVMThreadContext *tc, MVMString *handle);
MVMSerializationContext * MVM_sc_get_sc_slow(struct MVMThreadContext *tc, MVMCompUnit *cu, int16_t dep);
static inline MVMSerializationContext * MVM_sc_get_sc(struct MVMThreadContext *tc,
                                                          MVMCompUnit *cu, int16_t dep) {
    MVMSerializationContext *sc = cu->body.scs[dep];
    return sc ? sc : MVM_sc_get_sc_slow(tc, cu, dep);
}
static inline MVMObject * MVM_sc_get_sc_object(struct MVMThreadContext *tc, MVMCompUnit *cu,
                                 uint16_t dep, uint64_t idx) {
    MVMSerializationContext *sc = MVM_sc_get_sc(tc, cu, dep);
    if (MVM_UNLIKELY(sc == NULL))
        MVM_exception_throw_adhoc(tc, "SC not yet resolved; lookup failed");
    return MVM_sc_get_object(tc, sc, idx);
}
void MVM_sc_disclaim(struct MVMThreadContext *tc, MVMSerializationContext *sc);

static inline uint32_t MVM_sc_get_idx_of_sc(MVMCollectable *col) {
    assert(!(col->flags2 & MVM_CF_FORWARDER_VALID));
#ifdef MVM_USE_OVERFLOW_SERIALIZATION_INDEX
    if (col->flags1 & MVM_CF_SERIALZATION_INDEX_ALLOCATED)
        return col->sc_forward_u.sci->sc_idx;
#endif
    return col->sc_forward_u.sc.sc_idx;
}

static inline uint32_t MVM_sc_get_idx_in_sc(MVMCollectable *col) {
    assert(!(col->flags2 & MVM_CF_FORWARDER_VALID));
#ifdef MVM_USE_OVERFLOW_SERIALIZATION_INDEX
    if (col->flags1 & MVM_CF_SERIALZATION_INDEX_ALLOCATED)
        return col->sc_forward_u.sci->idx;
    if (col->sc_forward_u.sc.idx == MVM_DIRECT_SC_IDX_SENTINEL)
        return ~0;
#endif
    return col->sc_forward_u.sc.idx;
}

static inline void MVM_sc_set_idx_in_sc(MVMCollectable *col, uint32_t i) {
    assert(!(col->flags2 & MVM_CF_FORWARDER_VALID));
#ifdef MVM_USE_OVERFLOW_SERIALIZATION_INDEX
    if (col->flags1 & MVM_CF_SERIALZATION_INDEX_ALLOCATED) {
        col->sc_forward_u.sci->idx = i;
    } else if (i >= MVM_DIRECT_SC_IDX_SENTINEL) {
        struct MVMSerializationIndex *const sci
            = MVM_malloc(sizeof(struct MVMSerializationIndex));
        sci->sc_idx = col->sc_forward_u.sc.sc_idx;
        sci->idx = i;
        col->sc_forward_u.sci = sci;
        col->flags1 |= MVM_CF_SERIALZATION_INDEX_ALLOCATED;
    } else
#endif
    {
        col->sc_forward_u.sc.idx = i;
    }
}

/* Gets a collectable's SC. */
static inline MVMSerializationContext * MVM_sc_get_collectable_sc(struct MVMThreadContext *tc, MVMCollectable *col) {
    uint32_t sc_idx;
    assert(!(col->flags2 & MVM_CF_FORWARDER_VALID));
    sc_idx = MVM_sc_get_idx_of_sc(col);
    assert(sc_idx != ~(uint32_t)0);
    return sc_idx > 0 ? tc->instance->all_scs[sc_idx]->sc : NULL;
}

/* Gets an object's SC. */
static inline MVMSerializationContext * MVM_sc_get_obj_sc(struct MVMThreadContext *tc, MVMObject *obj) {
    return MVM_sc_get_collectable_sc(tc, &obj->header);
}

/* Gets a frame's SC. */
static inline MVMSerializationContext * MVM_sc_get_frame_sc(struct MVMThreadContext *tc, MVMFrame *f) {
    return MVM_sc_get_collectable_sc(tc, &f->header);
}

/* Gets an STables's SC. */
static inline MVMSerializationContext * MVM_sc_get_stable_sc(struct MVMThreadContext *tc, MVMSTable *st) {
    return MVM_sc_get_collectable_sc(tc, &st->header);
}

/* Sets a collectable's SC. */
static inline void MVM_sc_set_collectable_sc(struct MVMThreadContext *tc, MVMCollectable *col, MVMSerializationContext *sc) {
    assert(!(col->flags2 & MVM_CF_FORWARDER_VALID));
#ifdef MVM_USE_OVERFLOW_SERIALIZATION_INDEX
    if (col->flags1 & MVM_CF_SERIALZATION_INDEX_ALLOCATED) {
        col->sc_forward_u.sci->sc_idx = sc->body->sc_idx;
        col->sc_forward_u.sci->idx    = ~0;
    } else
#endif
    {
        col->sc_forward_u.sc.sc_idx = sc->body->sc_idx;
#ifdef MVM_USE_OVERFLOW_SERIALIZATION_INDEX
        if (col->sc_forward_u.sc.sc_idx != sc->body->sc_idx) {
            struct MVMSerializationIndex *const sci
                = MVM_malloc(sizeof(struct MVMSerializationIndex));
            sci->sc_idx = sc->body->sc_idx;
            sci->idx = ~0;
            col->sc_forward_u.sci = sci;
            col->flags1 |= MVM_CF_SERIALZATION_INDEX_ALLOCATED;
        } else
#endif
        {
            col->sc_forward_u.sc.idx    = MVM_DIRECT_SC_IDX_SENTINEL;
        }
    }
}

/* Sets an object's SC. */
static inline void MVM_sc_set_obj_sc(struct MVMThreadContext *tc, MVMObject *obj, MVMSerializationContext *sc) {
    MVM_sc_set_collectable_sc(tc, &obj->header, sc);
}

/* Sets an frame's SC. */
static inline void MVM_sc_set_frame_sc(struct MVMThreadContext *tc, MVMFrame *f, MVMSerializationContext *sc) {
    MVM_sc_set_collectable_sc(tc, &f->header, sc);
}

/* Sets an STable's SC. */
static inline void MVM_sc_set_stable_sc(struct MVMThreadContext *tc, MVMSTable *st, MVMSerializationContext *sc) {
    MVM_sc_set_collectable_sc(tc, &st->header, sc);
}

/* Given an SC, an index and a code ref, store it and the index. */
static inline void MVM_sc_set_code(struct MVMThreadContext *tc, MVMSerializationContext *sc, int64_t idx, MVMObject *code) {
    MVMObject *roots = sc->body->root_codes;
    MVM_repr_bind_pos_o(tc, roots, idx, code);
    if (MVM_sc_get_idx_of_sc(&code->header) == sc->body->sc_idx)
        MVM_sc_set_idx_in_sc(&code->header, idx);
}

/* Sets the full list of code refs. */
static inline void MVM_sc_set_code_list(struct MVMThreadContext *tc, MVMSerializationContext *sc, MVMObject *code_list) {
    MVM_ASSIGN_REF(tc, &(sc->common.header), sc->body->root_codes, code_list);
}

/* Gets the number of objects in the SC. */
static inline uint64_t MVM_sc_get_object_count(struct MVMThreadContext *tc, MVMSerializationContext *sc) {
    return sc->body->num_objects;
}

/* Given an SC and an object, push it onto the SC. */
static inline void MVM_sc_push_object(struct MVMThreadContext *tc, MVMSerializationContext *sc, MVMObject *obj) {
    uint32_t idx = sc->body->num_objects;
    MVM_sc_set_object(tc, sc, idx, obj);
    if (MVM_sc_get_idx_of_sc(&obj->header) == sc->body->sc_idx)
        MVM_sc_set_idx_in_sc(&obj->header, idx);
}

/* SC repossession write barriers. */
void MVM_sc_wb_hit_obj(struct MVMThreadContext *tc, MVMObject *obj);
void MVM_sc_wb_hit_st(struct MVMThreadContext *tc, MVMSTable *st);

void MVM_SC_WB_OBJ(struct MVMThreadContext *tc, MVMObject *obj);

static inline void MVM_SC_WB_ST(struct MVMThreadContext *tc, MVMSTable *st) {
    assert(!(st->header.flags2 & MVM_CF_FORWARDER_VALID));
    assert(MVM_sc_get_idx_of_sc(&st->header) != ~(uint32_t)0);
    if (MVM_sc_get_idx_of_sc(&st->header) > 0)
        MVM_sc_wb_hit_st(tc, st);
}
