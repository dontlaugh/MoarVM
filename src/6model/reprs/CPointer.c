#include "moar.h"

/* This representation's function pointer table. */
static const MVMREPROps CPointer_this_repr;

/* Creates a new type object of this representation, and associates it with
 * the given HOW. */
static MVMObject * type_object_for(struct MVMThreadContext *tc, MVMObject *HOW) {
    MVMSTable *st  = MVM_gc_allocate_stable(tc, &CPointer_this_repr, HOW);

    MVMROOT(tc, st) {
        MVMObject *obj = MVM_gc_allocate_type_object(tc, st);
        MVM_ASSIGN_REF(tc, &(st->header), st->WHAT, obj);
        st->size = sizeof(MVMCPointer);
    }

    return st->WHAT;
}

/* Compose the representation. */
static void compose(struct MVMThreadContext *tc, MVMSTable *st, MVMObject *info) {
}

/* Copies to the body of one object to another. */
static void copy_to(struct MVMThreadContext *tc, MVMSTable *st, void *src, MVMObject *dest_root, void *dest) {
    MVMCPointerBody *src_body = (MVMCPointerBody *)src;
    MVMCPointerBody *dest_body = (MVMCPointerBody *)dest;
    dest_body->ptr = src_body->ptr;
}

static void set_int(struct MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data, int64_t value) {
    MVMCPointerBody *body = (MVMCPointerBody *)OBJECT_BODY(root);
#if MVM_PTR_SIZE == 4
    body->ptr = (void *)(int32_t)value;
#else
    body->ptr = (void *)value;
#endif
}

static int64_t get_int(struct MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data) {
    MVMCPointerBody *body = (MVMCPointerBody *)OBJECT_BODY(root);
#if MVM_PTR_SIZE == 4
    return (int64_t)(int32_t)body->ptr;
#else
    return (int64_t)body->ptr;
#endif
}

static void set_uint(struct MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data, uint64_t value) {
    MVMCPointerBody *body = (MVMCPointerBody *)OBJECT_BODY(root);
#if MVM_PTR_SIZE == 4
    body->ptr = (void *)(uint32_t)value;
#else
    body->ptr = (void *)value;
#endif
}

static uint64_t get_uint(struct MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data) {
    MVMCPointerBody *body = (MVMCPointerBody *)OBJECT_BODY(root);
#if MVM_PTR_SIZE == 4
    return (uint64_t)(uint32_t)body->ptr;
#else
    return (uint64_t)body->ptr;
#endif
}

static const MVMStorageSpec storage_spec = {
    MVM_STORAGE_SPEC_REFERENCE,       /* inlineable */
    sizeof(void *) * 8,               /* bits */
    ALIGNOF(void *),                  /* align */
    MVM_STORAGE_SPEC_BP_NONE,         /* boxed_primitive */
    0,                                /* can_box */
    0,                                /* is_unsigned */
};


/* Gets the storage specification for this representation. */
static const MVMStorageSpec * get_storage_spec(struct MVMThreadContext *tc, MVMSTable *st) {
    return &storage_spec;
}

static void deserialize_stable_size(struct MVMThreadContext *tc, MVMSTable *st, MVMSerializationReader *reader) {
    st->size = sizeof(MVMCPointer);
}

static void deserialize(struct MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data, MVMSerializationReader *reader) {
    MVMCPointerBody *body = (MVMCPointerBody *)data;
    int64_t value;

    if (reader->root.version >= 19) {
        value = MVM_serialization_read_int(tc, reader);
    } else {
        value = MVM_serialization_read_int64(tc, reader);
    }

#if MVM_PTR_SIZE == 4
    body->ptr = (void *)(uint32_t)value;
#else
    body->ptr = (void *)value;
#endif

}

static void serialize(struct MVMThreadContext *tc, MVMSTable *st, void *data, MVMSerializationWriter *writer) {
    MVMCPointerBody *body = (MVMCPointerBody *)data;
    MVM_serialization_write_int(tc, writer,
#if MVM_PTR_SIZE == 4
        (uint64_t)(uint32_t)body->ptr
#else
        (uint64_t)body->ptr
#endif
    );
}

/* Initializes the representation. */
const MVMREPROps * MVMCPointer_initialize(struct MVMThreadContext *tc) {
    return &CPointer_this_repr;
}

static const MVMREPROps CPointer_this_repr = {
    type_object_for,
    MVM_gc_allocate_object,
    NULL, /* initialize */
    copy_to,
    MVM_REPR_DEFAULT_ATTR_FUNCS,
    {
        set_int,
        get_int,
        MVM_REPR_DEFAULT_SET_NUM,
        MVM_REPR_DEFAULT_GET_NUM,
        MVM_REPR_DEFAULT_SET_STR,
        MVM_REPR_DEFAULT_GET_STR,
        set_uint,
        get_uint,
        MVM_REPR_DEFAULT_GET_BOXED_REF
    },    /* box_funcs */
    MVM_REPR_DEFAULT_POS_FUNCS,
    MVM_REPR_DEFAULT_ASS_FUNCS,
    MVM_REPR_DEFAULT_ELEMS,
    get_storage_spec,
    NULL, /* change_type */
    serialize, /* serialize */
    deserialize, /* deserialize */
    NULL, /* serialize_repr_data */
    NULL, /* deserialize_repr_data */
    deserialize_stable_size,
    NULL, /* gc_mark */
    NULL, /* gc_free */
    NULL, /* gc_cleanup */
    NULL, /* gc_mark_repr_data */
    NULL, /* gc_free_repr_data */
    compose,
    NULL, /* spesh */
    "CPointer", /* name */
    MVM_REPR_ID_MVMCPointer,
    NULL, /* unmanaged_size */
    NULL, /* describe_refs */
};
