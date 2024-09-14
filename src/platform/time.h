/* Gets time since the epoch in nanoseconds.
 * In principle, may return 0 on error.
 */
uint64_t MVM_platform_now(void);

/* Tries to sleep for at least the requested number
 * of nanoseconds.
 */
void MVM_platform_sleep(double second);
void MVM_platform_nanosleep(uint64_t nanos);

void MVM_platform_decodelocaltime(struct MVMThreadContext *tc, int64_t time, int64_t decoded[]);
