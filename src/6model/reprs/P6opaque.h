/* This is how an instance with the P6opaque representation starts. However, what
 * follows on from this depends on the declaration. For object attributes, it will
 * be a pointer size and point to another MVMObject. For native integers and
 * numbers, it will be the appropriate sized piece of memory to store them
 * right there in the object. Note that P6opaque does not do packed storage, so
 * an int2 gets as much space as an int. */
struct MVMP6opaqueBody {
    /* If we get mixed into, we may change size. If so, we can't really resize
     * the object, so instead we hang its post-resize form off this pointer.
     * In the future, more clever things are possible (like only putting the
     * new fields into this object). */
    void *replaced;
};
struct MVMP6opaque {
    MVMObject common;
    MVMP6opaqueBody body;
};

/* This is used in the name to slot mapping. Indicates the class key that
 * we have the mappings for, followed by arrays of names and slots. (Yeah,
 * could use a hash, but much code will resolve these statically to the
 * slots). */
struct MVMP6opaqueNameMap {
    MVMObject  *class_key;
    MVMString **names;
    uint16_t  *slots;
    uint32_t   num_attrs;
};

/* The P6opaque REPR data has the slot mapping, allocation size and
 * various other bits of info. It hangs off the REPR_data pointer
 * in the s-table. */
struct MVMP6opaqueREPRData {
    /* The number of attributes we have allocated slots for. Note that
     * slots can vary in size. */
    uint16_t num_attributes;

    /* Slot containing object to delegate for positional things. */
    int16_t pos_del_slot;

    /* Slot containing object to delegate for associative things. */
    int16_t ass_del_slot;

    /* Flags if we are MI or not. */
    uint16_t mi;

    /* Slot to delegate to when we need to unbox to a native integer. */
    int16_t unbox_int_slot;

    /* Slot to delegate to when we need to unbox to a native unsigned integer. */
    int16_t unbox_uint_slot;

    /* Slot to delegate to when we need to unbox to a native number. */
    int16_t unbox_num_slot;

    /* Slot to delegate to when we need to unbox to a native string. */
    int16_t unbox_str_slot;

    /* Offsets into the object that are eligible for GC marking, and how
     * many of them we have. */
    uint16_t gc_obj_mark_offsets_count;
    uint16_t *gc_obj_mark_offsets;

    /* Maps attribute position numbers to the byte offset in the object. */
    uint16_t *attribute_offsets;

    /* If the attribute was actually flattened in to this object from another
     * representation, this is the s-table of the type of that attribute. NULL
     * for attributes that are just reference types. */
    MVMSTable **flattened_stables;

    /* Instantiated objects are just a blank piece of memory that needs to
     * be set up. However, in some cases we'd like them to magically turn in
     * to some container type. */
    MVMObject **auto_viv_values;

    /* If we have any other flattened boxings, this array can be indexed by
     * REPR ID to find the slot in the object where it is embedded. */
    uint16_t *unbox_slots;

    /* A table mapping attribute names to indexes (which can then be looked
     * up in the offset table). Uses a final null entry as a sentinel. */
    MVMP6opaqueNameMap *name_to_index_mapping;

    /* Slots holding flattened objects that need another REPR to initialize
     * them; terminated with -1. */
    int16_t *initialize_slots;

    /* Slots holding flattened objects that need another REPR to mark them;
     * terminated with -1. */
    int16_t *gc_mark_slots;

    /* Slots holding flattened objects that need another REPR to clean them;
     * terminated with -1. */
    int16_t *gc_cleanup_slots;

    /* Hold the storage spec */
    MVMStorageSpec storage_spec;
};

/* Function for REPR setup. */
const MVMREPROps * MVMP6opaque_initialize(struct MVMThreadContext *tc);

/* If an object gets mixed in to, we need to be sure we look at its real body,
 * which may have been moved to hang off the specified pointer.
 *
 * NB: This has been hardcoded into the jit compilation. Thus, consider it
 * set into stone :-). That is the price you pay for disintermediation. */
static inline void * MVM_p6opaque_real_data(struct MVMThreadContext *tc, void *data) {
    MVMP6opaqueBody *body = (MVMP6opaqueBody *)data;
    return body->replaced ? body->replaced : data;
}

/* Reads an attribute using an offset. This is only safe on an exact type
 * match. */
static inline MVMObject * MVM_p6opaque_read_object(struct MVMThreadContext *tc,
                                                       MVMObject *o, size_t offset) {
    char *data  = MVM_p6opaque_real_data(tc, OBJECT_BODY(o));
    return *((MVMObject **)(data + offset));
}
static inline int64_t MVM_p6opaque_read_int64(struct MVMThreadContext *tc,
                                                   MVMObject *o, size_t offset) {
    char *data  = MVM_p6opaque_real_data(tc, OBJECT_BODY(o));
    return *((int64_t *)(data + offset));
}
static inline double MVM_p6opaque_read_num64(struct MVMThreadContext *tc,
                                                   MVMObject *o, size_t offset) {
    char *data  = MVM_p6opaque_real_data(tc, OBJECT_BODY(o));
    return *((double *)(data + offset));
}
static inline MVMString * MVM_p6opaque_read_str(struct MVMThreadContext *tc,
                                                    MVMObject *o, size_t offset) {
    char *data  = MVM_p6opaque_real_data(tc, OBJECT_BODY(o));
    return *((MVMString **)(data + offset));
}

size_t MVM_p6opaque_attr_offset(struct MVMThreadContext *tc, MVMObject *type,
    MVMObject *class_handle, MVMString *name);
void MVM_p6opaque_attr_offset_and_arg_type(struct MVMThreadContext *tc, MVMObject *type,
    MVMObject *class_handle, MVMString *name, size_t *offset_out, MVMCallsiteFlags *type_out);
uint16_t MVM_p6opaque_get_bigint_offset(struct MVMThreadContext *tc, MVMSTable *st);
uint32_t MVM_p6opaque_offset_to_attr_idx(struct MVMThreadContext *tc, MVMObject *type, size_t offset);
void MVM_P6opaque_at_pos(struct MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data, int64_t index, MVMRegister *value, uint16_t kind);
