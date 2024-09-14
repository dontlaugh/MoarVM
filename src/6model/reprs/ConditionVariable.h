/* Representation used for condition variables. */
struct MVMConditionVariableBody {
    /* The ReentrantMutex this condition variable is associated with. */
    MVMObject *mutex;

    /* The condition variable itself, held at a level of indirection to keep
     * OSes that wouldn't like it moving around happy. */
    uv_cond_t *condvar;
};
struct MVMConditionVariable {
    MVMObject common;
    MVMConditionVariableBody body;
};

/* Function for REPR setup. */
const MVMREPROps * MVMConditionVariable_initialize(struct MVMThreadContext *tc);

/* Operations on a condition variable. */
MVMObject * MVM_conditionvariable_from_lock(struct MVMThreadContext *tc, MVMReentrantMutex *lock, MVMObject *type);
void MVM_conditionvariable_wait(struct MVMThreadContext *tc, MVMConditionVariable *cv);
void MVM_conditionvariable_signal_one(struct MVMThreadContext *tc, MVMConditionVariable *cv);
void MVM_conditionvariable_signal_all(struct MVMThreadContext *tc, MVMConditionVariable *cv);
