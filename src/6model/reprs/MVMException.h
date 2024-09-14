/* Representation for an exception in MoarVM. */
struct MVMExceptionBody {
    /* The exception message. */
    MVMString *message;

    /* The payload (object thrown with the exception). */
    MVMObject *payload;

    /* The exception category. */
    int32_t category;

    /* Flag indicating if we should return after unwinding. */
    uint8_t return_after_unwind;

    /* Where was the exception thrown from? */
    MVMFrame *origin;
    uint8_t *throw_address;

    /* Where should we resume to, if it's possible? */
    uint8_t *resume_addr;
    void     *jit_resume_label;
};
struct MVMException {
    MVMObject common;
    MVMExceptionBody body;
};

/* Function for REPR setup. */
const MVMREPROps * MVMException_initialize(MVMThreadContext *tc);
