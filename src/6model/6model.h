/* Boolification mode flags. */
#pragma once

#include <stdatomic.h>

#define MVM_BOOL_MODE_CALL_METHOD                   0
#define MVM_BOOL_MODE_UNBOX_INT                     1
#define MVM_BOOL_MODE_UNBOX_NUM                     2
#define MVM_BOOL_MODE_UNBOX_STR_NOT_EMPTY           3
#define MVM_BOOL_MODE_UNBOX_STR_NOT_EMPTY_OR_ZERO   4
#define MVM_BOOL_MODE_NOT_TYPE_OBJECT               5
#define MVM_BOOL_MODE_BIGINT                        6
#define MVM_BOOL_MODE_ITER                          7
#define MVM_BOOL_MODE_HAS_ELEMS                     8

/* Controls the way that type checks are performed. By default, if there is
 * a type check cache we treat it as definitive. However, it's possible to
 * declare that in the case the type check cache has no entry we should fall
 * back to asking the .HOW.type_check method (set TYPE_CHECK_CACHE_THEN_METHOD).
 * While a normal type check asks a value if it supports another type, the
 * TYPE_CHECK_NEEDS_ACCEPTS flag results in a call to .accepts_type on the
 * HOW of the thing we're checking the value against, giving it a chance to
 * decide answer. These are set as the lower bits of mode_flags in MVMSTable. */
#define MVM_TYPE_CHECK_CACHE_DEFINITIVE    0
#define MVM_TYPE_CHECK_CACHE_THEN_METHOD   1
#define MVM_TYPE_CHECK_NEEDS_ACCEPTS       2
#define MVM_TYPE_CHECK_CACHE_FLAG_MASK     3

/* This STable mode flag is set if the type needs finalization. */
#define MVM_FINALIZE_TYPE                  8

/* This STable mode flag is set if the type is parametric (and so can be
 * parameterized). */
#define MVM_PARAMETRIC_TYPE                 16

/* This STable mode flag is set if the type is a parameterization of some
 * parametric type. */
#define MVM_PARAMETERIZED_TYPE              32

/* STable mode flag for a type that is not subject to repossession. */
#define MVM_NEVER_REPOSSESS_TYPE            64

/* HLL type roles. */
#define MVM_HLL_ROLE_NONE                   0
#define MVM_HLL_ROLE_INT                    1
#define MVM_HLL_ROLE_NUM                    2
#define MVM_HLL_ROLE_STR                    3
#define MVM_HLL_ROLE_ARRAY                  4
#define MVM_HLL_ROLE_HASH                   5
#define MVM_HLL_ROLE_CODE                   6

/* Hint value to indicate the absence of an attribute lookup or method
 * dispatch hint. */
#define MVM_NO_HINT -1

/* This data structure describes what storage a given representation
 * needs if something of that representation is to be embedded in
 * another place. For any representation that expects to be used
 * as a kind of reference type, it will just want to be a pointer.
 * But for other things, they would prefer to be "inlined" into
 * the object. */
struct MVMStorageSpec {
    /* 0 if this is to be referenced, anything else otherwise. */
    uint16_t inlineable;

    /* For things that want to be inlined, the number of bits of
     * storage they need and what kind of byte-boundary they want to
     * be aligned to. Ignored otherwise. */
    uint16_t bits;
    uint16_t align;

    /* For things that are inlined, if they are just storage of a
     * primitive type and can unbox, this says what primitive type
     * that they unbox to. */
    uint16_t boxed_primitive;

    /* The types that this one can box/unbox to. */
    uint16_t can_box;

    /* For ints, whether it's an unsigned value. */
    uint8_t is_unsigned;
};

/* Inlined or not. */
#define MVM_STORAGE_SPEC_REFERENCE      0
#define MVM_STORAGE_SPEC_INLINED        1

/* Possible options for boxed primitives. */
#define MVM_STORAGE_SPEC_BP_NONE        0
#define MVM_STORAGE_SPEC_BP_INT         1
#define MVM_STORAGE_SPEC_BP_NUM         2
#define MVM_STORAGE_SPEC_BP_STR         3
#define MVM_STORAGE_SPEC_BP_INT8        4
#define MVM_STORAGE_SPEC_BP_INT16       5
#define MVM_STORAGE_SPEC_BP_INT32       6
#define MVM_STORAGE_SPEC_BP_UINT8       7
#define MVM_STORAGE_SPEC_BP_UINT16      8
#define MVM_STORAGE_SPEC_BP_UINT32      9
#define MVM_STORAGE_SPEC_BP_UINT64     10

/* can_box bit field values. */
#define MVM_STORAGE_SPEC_CAN_BOX_INT    1
#define MVM_STORAGE_SPEC_CAN_BOX_NUM    2
#define MVM_STORAGE_SPEC_CAN_BOX_STR    4
#define MVM_STORAGE_SPEC_CAN_BOX_MASK   7

/* Flags that may be set on any collectable.
 * These are split across two bytes so that we can set MVM_CF_HAS_OBJECT_ID and
 * MVM_CF_REF_FROM_GEN2 from different threads without a data race.
 * (Plenty of work for the CPU cache coherency hardware, but no race.)
 * This avoids the need to use atomic operations to manipulate the flags. */

/* Needed to provide a migration path for the code in Rakudo's perl6_ops.c */
#define MVM_COLLECTABLE_FLAGS1

typedef enum {
    /* Is a type object (and thus not a concrete instance). */
    MVM_CF_TYPE_OBJECT = 1,

    /* Is an STable. */
    MVM_CF_STABLE = 2,

    /* Is a heap-promoted call frame.
     * Beware, this flag bit must not overlap with any values set by macros
     * MVM_FRAME_FLAG_* in frame.h */
    MVM_CF_FRAME = 4,

    /* Have we allocated memory to store a serialization index? */
    MVM_CF_SERIALZATION_INDEX_ALLOCATED = 8,

    /* Have we arranged a persistent object ID for this object? */
    MVM_CF_HAS_OBJECT_ID = 16,

    /* Have we flagged this object as something we must never repossess? */
    /* Note: if you're hunting for a flag, some day in the future when we
     * have used them all, this one is easy enough to eliminate by having the
     * tiny number of objects marked this way in a remembered set. */
    MVM_CF_NEVER_REPOSSESS = 32
} MVMCollectableFlags1;

typedef enum {
    /* Has already been seen once in GC nursery. */
    MVM_CF_NURSERY_SEEN = 1,

    /* Has been promoted to the old generation. */
    MVM_CF_SECOND_GEN = 2,

    /* Has already been added to the gen2 aggregates pointing to nursery
     * objects list. */
    MVM_CF_IN_GEN2_ROOT_LIST = 4,

    /* A full GC run has found this object to be live. */
    MVM_CF_GEN2_LIVE = 8,

    /* This object in fromspace is live with a valid forwarder. */
    /* TODO - should be possible to use the same bit for this and GEN2_LIVE. */
    MVM_CF_FORWARDER_VALID = 16,

    /* Is this object a nursery object that has been referenced from gen2?
     * Used to promote it earlier. */
    MVM_CF_REF_FROM_GEN2 = 32,

    /* Has this item been chained into a gen2 freelist? This is only used in
     * GC debug more. */
    MVM_CF_DEBUG_IN_GEN2_FREE_LIST = 64
} MVMCollectableFlags2;

#ifdef MVM_USE_OVERFLOW_SERIALIZATION_INDEX
struct MVMSerializationIndex {
    uint32_t sc_idx;
    uint32_t idx;
};
#endif

/* Things that every GC-collectable entity has. These fall into two
 * categories:
 *   * MVMObject - objects. Almost everything is one of these.
 *   * MVMSTable - shared tables; one per (HOW, REPR) pairing.
 * Only the first can vary in size, and even then only if it's not a
 * type object.
 */
struct MVMCollectable {
    /* Put this union first, as these pointers/indexes are relatively "cold",
       whereas "flags" is accessed relatively frequently, as are the fields
       that follow in the structures into which MVMCollectable is embedded.
       Shrinking the size of the active part of the structure slightly
       increases the chance that it fits into the CPU's L1 cache, which is a
       "free" performance win. */
    union {
        /* Forwarding pointer, for copying/compacting GC purposes. */
        MVMCollectable *forwarder;
        /* Index of the serialization context this collectable lives in, if
         * any, and then location within that. */
#ifdef MVM_USE_OVERFLOW_SERIALIZATION_INDEX
        struct {
            uint16_t sc_idx;
            uint16_t idx;
        } sc;
        struct MVMSerializationIndex *sci;
#else
        struct {
            uint32_t sc_idx;
            uint32_t idx;
        } sc;
#endif
        /* Used to chain STables queued to be freed. */
        MVMSTable *st;
    } sc_forward_u;

    /* Identifier of the thread that created the object. 0 if this is a
     * non-heap frame. */
    uint32_t owner;

    /* Collectable flags (see MVMCollectableFlags). */
    uint8_t flags1;
    uint8_t flags2;

    /* Object size, in bytes. */
    uint16_t size;
};
#ifdef MVM_USE_OVERFLOW_SERIALIZATION_INDEX
#  define MVM_DIRECT_SC_IDX_SENTINEL 0xFFFF
#else
#  define MVM_DIRECT_SC_IDX_SENTINEL ~0
#endif

/* The common things every object has.
 *
 * NB - the assumption that MVMObject* can be safely cast into
 * MVMCollectable* is spread throughout the codebase, as well
 * as used directly in JIT. Thus, nothing may preceed the header!
 */
struct MVMObject {
    /* Commonalities that all collectable entities have. */
    MVMCollectable header;

    /* The s-table for the object. */
    MVMSTable *st;
};

/* An dummy object, mostly used to compute the offset of the data part of
 * a 6model object. */
struct MVMObjectStooge {
    MVMObject common;
    void *data;
};

/* This is used to identify an attribute for various types of cache. */
struct MVMAttributeIdentifier {
    MVMObject         *class_handle;   /* Class handle */
    MVMString         *attr_name;      /* Name of the attribute. */
    int64_t           hint;           /* Hint for use in static/gradual typing. */
};

/* How do we turn something of this type into a boolean? */
struct MVMBoolificationSpec {
    MVMObject *method;
    uint32_t  mode;
};

/* Constant for incrementing the type cache ID for new STables. This leaves
 * the lowest bits free for caches to attach flags (of note, the multi
 * dispatch cache). */
#define MVM_TYPE_CACHE_ID_INCR 256

/* S-table, representing a meta-object/representation pairing. Note that the
 * items are grouped in hope that it will pack decently and do decently in
 * terms of cache lines. */
struct MVMSTable {
    /* Commonalities that all collectable entities have. */
    MVMCollectable header;

    /* The representation operation table. */
    const MVMREPROps *REPR;

    /* Any data specific to this type that the REPR wants to keep. */
    void *REPR_data;

    /* The size of an object of this type in bytes, including the
     * header. */
    uint32_t size;

    /* The length of the type check cache. */
    uint16_t type_check_cache_length;

    /* The type checking mode and method cache mode (see flags for this
     * above). */
    uint16_t mode_flags;

    /* Array of type objects. If this is set, then it is expected to contain
     * the type objects of all types that this type is equivalent to (e.g.
     * all the things it isa and all the things it does). */
    MVMObject **type_check_cache;

    /* An ID solely for use in caches that last a VM instance. Thus it
     * should never, ever be serialized and you should NEVER make a
     * type directory based upon this ID. Otherwise you'll create memory
     * leaks for anonymous types, and other such screwups. */
    uint64_t type_cache_id;

    /* If this is a container, then this contains information needed in
     * order to fetch the value in it. If not, it'll be null, which can
     * be taken as a "not a container" indication. */
    const MVMContainerSpec *container_spec;

    /* Data that the container spec may need to function. */
    /* Any data specific to this type that the REPR wants to keep. */
    void *container_data;

    /* Information - if any - about how we can turn something of this type
     * into a boolean. */
    MVMBoolificationSpec *boolification_spec;

    /* The HLL that this type is owned by, if any. */
    MVMHLLConfig *hll_owner;

    /* The role that the type plays in the HLL, if any. */
    int64_t hll_role;

    /* The type-object. */
    MVMObject *WHAT;

    /* The underlying package stash. */
    MVMObject *WHO;

    /* The meta-object. */
    MVMObject *HOW;

    /* Parametricity. Mode flags indicate what, if any, of this union is valid. */
    union {
        struct {
            /* The code handle to use to produce a new parameterization. */
            MVMCode *parameterizer;

            /* Lookup table of existing parameterizations. For now, just a VM
             * array with alternating pairs of [arg array], object. Could in
             * the future we something lower level or hashy; we've yet to see
             * how hot-path lookups end up being in reality. */
            MVMObject *lookup;
        } ric;
        struct {
            /* The type that we are a parameterization of. */
            MVMObject *parametric_type;

            /* Our type parameters. */
            MVMObject *parameters;
        } erized;
    } paramet;

    /* We lazily deserialize HOW; this is the SC and index if needed. */
    MVMSerializationContext *HOW_sc;
    uint32_t                HOW_idx;

    /* A string associated with this STable for debugging purposes.
     * Usually the name of the class this belongs to. */
    char *debug_name;

    /* If this STable is currently in the process of being repossessed. Used
     * to trigger clearup of memory pre-repossession. */
    uint8_t being_repossessed;

    /* If this STable represents a type that can be the target of a
     * change_type - that is to say, it's been mixed in to. */
    uint8_t is_mixin_type;
};

/* The representation operations table. Note that representations are not
 * classes - there's no inheritance, so there's no polymorphism. If you know
 * a representation statically, you can statically dereference the call to
 * the representation op in question. In the dynamic case, you have to go
 * following the pointer, however. */
struct MVMREPROps_Attribute {
    /* Gets the current value for an attribute and places it in the passed
     * location (specified as a register). Expects to be passed a kind flag
     * that matches the kind of the attribute that is being fetched. */
    void (*get_attribute) (struct MVMThreadContext *tc, MVMSTable *st,
        MVMObject *root, void *data, MVMObject *class_handle, MVMString *name,
        int64_t hint, MVMRegister *result, uint16_t kind);

    /* Binds the given object or value to the specified attribute. The
     * kind flag specifies the type of value being passed to be bound.*/
    void (*bind_attribute) (struct MVMThreadContext *tc, MVMSTable *st,
        MVMObject *root, void *data, MVMObject *class_handle, MVMString *name,
        int64_t hint, MVMRegister value, uint16_t kind);

    /* Gets the hint for the given attribute ID. */
    int64_t (*hint_for) (struct MVMThreadContext *tc, MVMSTable *st,
        MVMObject *class_handle, MVMString *name);

    /* Checks if an attribute has been initialized. */
    int64_t (*is_attribute_initialized) (struct MVMThreadContext *tc, MVMSTable *st,
        void *data, MVMObject *class_handle, MVMString *name,
        int64_t hint);

    /* Provided the attribute is a native integer of the architecture's atomic
     * size or an object pointer, returns an atomic_uintptr_t * referencing it. This is only
     * valid until the next safepoint. If rebless is called on the object,
     * updates may be lost although memory safety must not be violated. */
    atomic_uintptr_t * (*attribute_as_atomic) (struct MVMThreadContext *tc, MVMSTable *st,
        void *data, MVMObject *class_handle, MVMString *name, uint16_t kind);
};
struct MVMREPROps_Boxing {
    /* Used with boxing. Sets an integer value, for representations that
     * can hold one. */
    void (*set_int) (struct MVMThreadContext *tc, MVMSTable *st,
        MVMObject *root, void *data, int64_t value);

    /* Used with boxing. Gets an integer value, for representations that
     * can hold one. */
    int64_t (*get_int) (struct MVMThreadContext *tc, MVMSTable *st,
        MVMObject *root, void *data);

    /* Used with boxing. Sets a floating point value, for representations that
     * can hold one. */
    void (*set_num) (struct MVMThreadContext *tc, MVMSTable *st,
        MVMObject *root, void *data, double value);

    /* Used with boxing. Gets a floating point value, for representations that
     * can hold one. */
    double (*get_num) (struct MVMThreadContext *tc, MVMSTable *st,
        MVMObject *root, void *data);

    /* Used with boxing. Sets a string value, for representations that
     * can hold one. */
    void (*set_str) (struct MVMThreadContext *tc, MVMSTable *st,
        MVMObject *root, void *data, MVMString *value);

    /* Used with boxing. Gets a string value, for representations that
     * can hold one. */
    MVMString * (*get_str) (struct MVMThreadContext *tc, MVMSTable *st,
        MVMObject *root, void *data);

    /* Used with boxing. Sets an unsinged integer value, for representations
     *  that can hold one. */
    void (*set_uint) (struct MVMThreadContext *tc, MVMSTable *st,
        MVMObject *root, void *data, uint64_t value);

    /* Used with boxing. Gets an unsigned integer value, for representations
     * that can hold one. */
    uint64_t (*get_uint) (struct MVMThreadContext *tc, MVMSTable *st,
        MVMObject *root, void *data);

    /* Some objects serve primarily as boxes of others, inlining them. This gets
     * gets the reference to such things, using the representation ID to distinguish
     * them. */
    void * (*get_boxed_ref) (struct MVMThreadContext *tc, MVMSTable *st,
        MVMObject *root, void *data, uint32_t repr_id);
};
struct MVMREPROps_Positional {
    /* Gets the element and the specified index and places it in the passed
     * location (specified as a register). Expects to be passed a kind flag
     * that matches the kind of the attribute that is being fetched. */
    void (*at_pos) (struct MVMThreadContext *tc, MVMSTable *st,
        MVMObject *root, void *data, int64_t index,
        MVMRegister *result, uint16_t kind);

    /* Binds the given object or value to the specified index. The
     * kind flag specifies the type of value being passed to be bound.*/
    void (*bind_pos) (struct MVMThreadContext *tc, MVMSTable *st,
        MVMObject *root, void *data, int64_t index,
        MVMRegister value, uint16_t kind);

    /* Sets the element count of the array, expanding or shrinking
     * it as needed. */
    void (*set_elems) (struct MVMThreadContext *tc, MVMSTable *st,
        MVMObject *root, void *data, uint64_t count);

    /* Pushes the specified value onto the array. */
    void (*push) (struct MVMThreadContext *tc, MVMSTable *st,
        MVMObject *root, void *data, MVMRegister value, uint16_t kind);

    /* Pops the value at the end of the array off it. */
    void (*pop) (struct MVMThreadContext *tc, MVMSTable *st,
        MVMObject *root, void *data, MVMRegister *value, uint16_t kind);

    /* Unshifts the value onto the array. */
    void (*unshift) (struct MVMThreadContext *tc, MVMSTable *st,
        MVMObject *root, void *data, MVMRegister value, uint16_t kind);

    /* Gets the value at the start of the array, and moves the starting point of
     * the array so that the next element is element zero. */
    void (*shift) (struct MVMThreadContext *tc, MVMSTable *st,
        MVMObject *root, void *data, MVMRegister *value, uint16_t kind);

    /* Creates a slice of the source array (from start to end) and stores it
     * within the destination array. If start or end is "-n", it will be
     * translated into the nth position relative to the end of the array. */
    void (*slice) (struct MVMThreadContext *tc, MVMSTable *st,
        MVMObject *src, void *data, MVMObject *dest,
        int64_t start, int64_t end);

    /* Splices the specified array into this one. Representations may optimize if
     * they know the type of the passed array, otherwise they should use the REPR
     * API. */
    void (*splice) (struct MVMThreadContext *tc, MVMSTable *st,
        MVMObject *root, void *data, MVMObject *from,
        int64_t offset, uint64_t elems);

    /* Multi-dimensional array read. */
    void (*at_pos_multidim) (struct MVMThreadContext *tc, MVMSTable *st,
        MVMObject *root, void *data, int64_t num_indices,
        int64_t *indices, MVMRegister *result, uint16_t kind);

    /* Multi-dimensional array write. */
    void (*bind_pos_multidim) (struct MVMThreadContext *tc, MVMSTable *st,
        MVMObject *root, void *data, int64_t num_indices,
        int64_t *indices, MVMRegister value, uint16_t kind);

    /* Gets the number of dimensions along with a C-level array of them. The
     * second two parameters are "out"s. The caller must not mutate dimensions,
     * nor persist it such that it lasts longer than the next VM safepoint. */
    void (*dimensions) (struct MVMThreadContext *tc, MVMSTable *st,
        MVMObject *root, void *data, int64_t *num_dimensions,
        int64_t **dimensions);

    /* Sets the number of dimensions. The caller is responsible for freeing
     * the array passed in dimensions. */
    void (*set_dimensions) (struct MVMThreadContext *tc, MVMSTable *st,
        MVMObject *root, void *data, int64_t num_dimensions,
        int64_t *dimensions);

    /* Gets the STable representing the declared element type. */
    MVMStorageSpec (*get_elem_storage_spec) (struct MVMThreadContext *tc, MVMSTable *st);

    /* Provided the array consists of integers of the architecture's atomic
     * size, gets an atomic_uintptr_t * pointing to that element and valid until the next
     * safepoint. */
    atomic_uintptr_t * (*pos_as_atomic) (struct MVMThreadContext *tc, MVMSTable *st, MVMObject *root,
        void *data, int64_t index);

    /* Multi-dim version of as_atomic. */
    atomic_uintptr_t * (*pos_as_atomic_multidim) (struct MVMThreadContext *tc, MVMSTable *st, MVMObject *root,
        void *data, int64_t num_indices, int64_t *indices);

    void (*write_buf) (struct MVMThreadContext *tc, MVMSTable *st,
        MVMObject *root, void *data, char *from,
        int64_t offset, uint64_t elems);

    int64_t (*read_buf) (struct MVMThreadContext *tc, MVMSTable *st,
        MVMObject *root, void *data, int64_t offset, uint64_t elems);
};
struct MVMREPROps_Associative {
    /* Gets the value at the specified key and places it in the passed
     * location (specified as a register). Expects to be passed a kind flag
     * that matches the kind of the attribute that is being fetched. */
    void (*at_key) (struct MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data,
        MVMObject *key, MVMRegister *result, uint16_t kind);

    /* Binds the object at the specified address into the hash at the specified
     * key. */
    void (*bind_key) (struct MVMThreadContext *tc, MVMSTable *st, MVMObject *root,
        void *data, MVMObject *key, MVMRegister value, uint16_t kind);

    /* Returns a true value of the key exists, and a false one if not. */
    int64_t (*exists_key) (struct MVMThreadContext *tc, MVMSTable *st,
        MVMObject *root, void *data, MVMObject *key);

    /* Deletes the specified key. */
    void (*delete_key) (struct MVMThreadContext *tc, MVMSTable *st,
        MVMObject *root, void *data, MVMObject *key);

    /* Gets the storage spec of the hash value type. */
    MVMStorageSpec (*get_value_storage_spec) (struct MVMThreadContext *tc, MVMSTable *st);
};
struct MVMREPROps {
    /* Creates a new type object of this representation, and
     * associates it with the given HOW. Also sets up a new
     * representation instance if needed. */
    MVMObject * (*type_object_for) (struct MVMThreadContext *tc, MVMObject *HOW);

    /* Allocates a new, but uninitialized object, based on the
     * specified s-table. */
    MVMObject * (*allocate) (struct MVMThreadContext *tc, MVMSTable *st);

    /* Used to initialize the body of an object representing the type
     * describe by the specified s-table. DATA points to the body. It
     * may recursively call initialize for any flattened objects. */
    void (*initialize) (struct MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data);

    /* For the given type, copies the object data from the source memory
     * location to the destination one. Note that it may actually be more
     * involved than a straightforward bit of copying; what's important is
     * that the representation knows about that. Note that it may have to
     * call copy_to recursively on representations of any flattened objects
     * within its body. */
    void (*copy_to) (struct MVMThreadContext *tc, MVMSTable *st, void *src, MVMObject *dest_root, void *dest);

    /* Attribute access REPR function table. */
    MVMREPROps_Attribute attr_funcs;

    /* Boxing REPR function table. */
    MVMREPROps_Boxing box_funcs;

    /* Positional indexing REPR function table. */
    MVMREPROps_Positional pos_funcs;

    /* Associative indexing REPR function table. */
    MVMREPROps_Associative ass_funcs;

    /* Gets the number of elements, for any aggregate types. */
    uint64_t (*elems) (struct MVMThreadContext *tc, MVMSTable *st,
        MVMObject *root, void *data);

    /* Gets the storage specification for this representation. */
    const MVMStorageSpec * (*get_storage_spec) (struct MVMThreadContext *tc, MVMSTable *st);

    /* Handles an object changing its type. The representation is responsible
     * for doing any changes to the underlying data structure, and may reject
     * changes that it's not willing to do (for example, a representation may
     * choose to only handle switching to a subclass). It is also left to update
     * the S-Table pointer as needed; while in theory this could be factored
     * out, the representation probably knows more about timing issues and
     * thread safety requirements. */
    void (*change_type) (struct MVMThreadContext *tc, MVMObject *object, MVMObject *new_type);

    /* Object serialization. Writes the object's body out using the passed
     * serialization writer. */
    void (*serialize) (struct MVMThreadContext *tc, MVMSTable *st, void *data, MVMSerializationWriter *writer);

    /* Object deserialization. Reads the object's body in using the passed
     * serialization reader. */
    void (*deserialize) (struct MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data, MVMSerializationReader *reader);

    /* REPR data serialization. Serializes the per-type representation data that
     * is attached to the supplied STable. */
    void (*serialize_repr_data) (struct MVMThreadContext *tc, MVMSTable *st, MVMSerializationWriter *writer);

    /* REPR data deserialization. Deserializes the per-type representation data and
     * attaches it to the supplied STable. */
    void (*deserialize_repr_data) (struct MVMThreadContext *tc, MVMSTable *st, MVMSerializationReader *reader);

    /* Deserialization of STable size. */
    void (*deserialize_stable_size) (struct MVMThreadContext *tc, MVMSTable *st, MVMSerializationReader *reader);

    /* MoarVM-specific REPR API addition used to mark an object. This involves
     * adding all pointers it contains to the worklist. */
    void (*gc_mark) (struct MVMThreadContext *tc, MVMSTable *st, void *data, MVMGCWorklist *worklist);

    /* MoarVM-specific REPR API addition used to free an object. */
    void (*gc_free) (struct MVMThreadContext *tc, MVMObject *object);

    /* This is called to do any cleanup of resources when an object gets
     * embedded inside another one. Never called on a top-level object. */
    void (*gc_cleanup) (struct MVMThreadContext *tc, MVMSTable *st, void *data);

    /* MoarVM-specific REPR API addition used to mark a REPR instance. */
    void (*gc_mark_repr_data) (struct MVMThreadContext *tc, MVMSTable *st, MVMGCWorklist *worklist);

    /* MoarVM-specific REPR API addition used to free a REPR instance. */
    void (*gc_free_repr_data) (struct MVMThreadContext *tc, MVMSTable *st);

    /* Causes the representation to be composed. Composition involves
     * passing the representation information that it needs in order
     * to compute memory layout. */
    void (*compose) (struct MVMThreadContext *tc, MVMSTable *st, MVMObject *info);

    /* Allows the REPR to produce specialized bytecode versions of various
     * instructions, when we know some of the types involved. */
    void (*spesh) (struct MVMThreadContext *tc, MVMSTable *st, MVMSpeshGraph *g,
        MVMSpeshBB *bb, MVMSpeshIns *ins);

    /* The representation's name. */
    const char *name;

    /* The representation's ID. */
    uint32_t ID;

    /* Optional API, for representations that allocate additonal memory and
     * want to report its size for debugging purposes. */
    uint64_t (*unmanaged_size) (struct MVMThreadContext *tc, MVMSTable *st, void *data);

    /* Optional API to describe references to other Collectables either by
     * index or by name, i.E. names of attributes or lexicals. */
    void (*describe_refs) (struct MVMThreadContext *tc, MVMHeapSnapshotState *ss, MVMSTable *st, void *data);
};

/* Various handy macros for getting at important stuff. */
#define STABLE(o)        (((MVMObject *)(o))->st)
#define REPR(o)          (STABLE((o))->REPR)
#define OBJECT_BODY(o)   (&(((MVMObjectStooge *)(o))->data))

/* Macros for getting/setting type-objectness. */
#define IS_CONCRETE(o)   (!(((MVMObject *)o)->header.flags1 & MVM_CF_TYPE_OBJECT))

/* Some functions related to 6model core functionality. */
 MVMObject * MVM_6model_get_how(struct MVMThreadContext *tc, MVMSTable *st);
 MVMObject * MVM_6model_get_how_obj(struct MVMThreadContext *tc, MVMObject *obj);
int64_t MVM_6model_try_cache_type_check(struct MVMThreadContext *tc, MVMObject *obj, MVMObject *type, int64_t *result);
void MVM_6model_stable_gc_free(struct MVMThreadContext *tc, MVMSTable *st);
uint64_t MVM_6model_next_type_cache_id(struct MVMThreadContext *tc);
void MVM_6model_never_repossess(struct MVMThreadContext *tc, MVMObject *obj);

static inline char *MVM_6model_get_debug_name(struct MVMThreadContext *tc, MVMObject *obj) {
    return obj ? STABLE(obj)->debug_name ? STABLE(obj)->debug_name : "" : "";
}
static inline char *MVM_6model_get_stable_debug_name(struct MVMThreadContext *tc, MVMSTable *stable) {
    return stable->debug_name ? stable->debug_name : "";
}
void MVM_6model_set_debug_name(struct MVMThreadContext *tc, MVMObject *type, MVMString *name);
