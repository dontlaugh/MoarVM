#define MVM_P6INT_C_TYPE_CHAR      -1
#define MVM_P6INT_C_TYPE_SHORT     -2
#define MVM_P6INT_C_TYPE_INT       -3
#define MVM_P6INT_C_TYPE_LONG      -4
#define MVM_P6INT_C_TYPE_LONGLONG  -5
#define MVM_P6INT_C_TYPE_SIZE_T    -6
#define MVM_P6INT_C_TYPE_BOOL      -7
#define MVM_P6INT_C_TYPE_ATOMIC    -8

/* Representation used by P6 native ints. */
struct MVMP6intBody {
    /* Integer storage slot. */
    union {
        int64_t  i64;
        int32_t  i32;
        int16_t  i16;
        int8_t   i8;
        uint64_t u64;
        uint32_t u32;
        uint16_t u16;
        uint8_t  u8;
    } value;
};
struct MVMP6int {
    MVMObject common;
    MVMP6intBody body;
};

/* The bit width requirement is shared for all instances of the same type. */
struct MVMP6intREPRData {
    int16_t       bits;
    int16_t       is_unsigned;
    MVMStorageSpec storage_spec;
};

/* Function for REPR setup. */
const MVMREPROps * MVMP6int_initialize(MVMThreadContext *tc);

void MVMP6int_set_int(MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data, int64_t value);
int64_t MVMP6int_get_int(MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data);
