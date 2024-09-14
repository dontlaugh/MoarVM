/* Representation used by the null REPR. */
struct MVMNull {
    MVMObject common;
};

/* Function for REPR setup. */
const MVMREPROps * MVMNull_initialize(struct MVMThreadContext *tc);

/* Macro for VM null checks. */
static inline int64_t MVM_is_null(struct MVMThreadContext *tc, MVMObject *check) {
    return !check || check == tc->instance->VMNull;
}
