/* Concurrent use of a VMArray is erroneous. This debugging option will
 * catch bad usages. (Eventually, we will refactor VMArray to not have
 * this issue.) */
#define MVM_ARRAY_CONC_DEBUG 0

/* Representation used by VM-level arrays. Adopted from QRPA work by
 * Patrick Michaud. */
struct MVMArrayBody {
    /* number of elements (from user's point of view) */
    uint64_t   elems;

    /* slot index of first element */
    uint64_t   start;

    /* size of slots array */
    uint64_t   ssize;

    /* slot array; union of various types of storage we may have. */
    union {
        MVMObject **o;
        MVMString **s;
        int64_t   *i64;
        int32_t   *i32;
        int16_t   *i16;
        int8_t    *i8;
        double   *n64;
        float   *n32;
        uint64_t  *u64;
        uint32_t  *u32;
        uint16_t  *u16;
        uint8_t   *u8;
        void       *any;
    } slots;

#if MVM_ARRAY_CONC_DEBUG
    AO_t in_use;
#endif 
};
struct MVMArray {
    MVMObject common;
    MVMArrayBody body;
};

/* Types of things we may be storing. */
#define MVM_ARRAY_OBJ   0
#define MVM_ARRAY_STR   1
#define MVM_ARRAY_I64   2
#define MVM_ARRAY_I32   3
#define MVM_ARRAY_I16   4
#define MVM_ARRAY_I8    5
#define MVM_ARRAY_N64   6
#define MVM_ARRAY_N32   7
#define MVM_ARRAY_U64   8
#define MVM_ARRAY_U32   9
#define MVM_ARRAY_U16   10
#define MVM_ARRAY_U8    11
#define MVM_ARRAY_U4    12
#define MVM_ARRAY_U2    13
#define MVM_ARRAY_U1    14
#define MVM_ARRAY_I4    15
#define MVM_ARRAY_I2    16
#define MVM_ARRAY_I1    17

/* Function for REPR setup. */
const MVMREPROps * MVMArray_initialize(MVMThreadContext *tc);

/* Array REPR data specifies the type of array elements we have. */
struct MVMArrayREPRData {
    /* The size of each element. */
    size_t elem_size;

    /* What type of slots we have. */
    uint8_t slot_type;

    /* Type object for the element type. */
    MVMObject *elem_type;
};
void MVM_VMArray_at_pos(MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data, int64_t index, MVMRegister *value, uint16_t kind);
void *MVM_VMArray_find_fast_impl_for_jit(MVMThreadContext *tc, MVMSTable *st, int16_t op, uint16_t kind);
void MVM_VMArray_bind_pos(MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data, int64_t index, MVMRegister value, uint16_t kind);

void MVM_VMArray_push(MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data, MVMRegister value, uint16_t kind);
