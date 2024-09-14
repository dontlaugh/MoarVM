/* Representation used for VM thread handles. */
struct MVMSemaphoreBody {
    uv_sem_t *sem;
};
struct MVMSemaphore {
    MVMObject common;
    MVMSemaphoreBody body;
};

/* Function for REPR setup. */
const MVMREPROps * MVMSemaphore_initialize(struct MVMThreadContext *tc);

/* Acquire and release functions. */
int64_t MVM_semaphore_tryacquire(struct MVMThreadContext *tc, MVMSemaphore *sem);
void MVM_semaphore_acquire(struct MVMThreadContext *tc, MVMSemaphore *sem);
void MVM_semaphore_release(struct MVMThreadContext *tc, MVMSemaphore *sem);
