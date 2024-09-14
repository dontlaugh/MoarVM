/* Representation used by the null REPR. */
struct MVMNull {
    MVMObject common;
};

/* Function for REPR setup. */
const MVMREPROps * MVMNull_initialize(MVMThreadContext *tc);

/* Macro for VM null checks. */
MVM_STATIC_INLINE int64_t MVM_is_null(MVMThreadContext *tc, MVMObject *check) {
    return !check || check == tc->instance->VMNull;
}
