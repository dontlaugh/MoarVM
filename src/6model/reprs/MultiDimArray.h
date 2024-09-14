/* Body of a multi-dim array is two blobs of memory: one holding the sizes of
 * the dimensions, and another holding the storage. The number of dimensions
 * is part of the type. */
struct MVMMultiDimArrayBody {
    /* The sizes of the dimensions. */
    int64_t *dimensions;

    /* 1D array of slots, which is the storage. We do the math on the
     * dimensions to get a mapping into here. Note that this memory is
     * fixed in size and never reallocated over the life of the array. */
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
};

struct MVMMultiDimArray {
    MVMObject common;
    MVMMultiDimArrayBody body;
};

/* REPR data specifies the type of array elements and number of dimensions we
 * have (the actual size of the dimensions is part of the value). */
struct MVMMultiDimArrayREPRData {
    /* Number of dimensions we have. */
    int64_t num_dimensions;

    /* The size of each element. */
    size_t elem_size;

    /* What type of slots we have. */
    uint8_t slot_type;

    /* Type object for the element type. */
    MVMObject *elem_type;
};

/* Initializes the MultiDimArray REPR. */
const MVMREPROps * MVMMultiDimArray_initialize(struct MVMThreadContext *tc);
