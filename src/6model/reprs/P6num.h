#define MVM_P6NUM_C_TYPE_FLOAT       -1
#define MVM_P6NUM_C_TYPE_DOUBLE      -2
#define MVM_P6NUM_C_TYPE_LONGDOUBLE  -3

/* Representation used by P6 nums. */
struct MVMP6numBody {
    /* Float storage slot. */
    union {
        double n64;
        float n32;
    } value;
};
struct MVMP6num {
    MVMObject common;
    MVMP6numBody body;
};

/* The bit width requirement is shared for all instances of the same type. */
struct MVMP6numREPRData {
    int16_t       bits;
    MVMStorageSpec storage_spec;
};

/* Function for REPR setup. */
const MVMREPROps * MVMP6num_initialize(MVMThreadContext *tc);
