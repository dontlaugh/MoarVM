/* Tries to determine the number of logical CPUs available to the process.
 * May return 0 on error.
 */
int64_t   MVM_platform_cpu_count(void);
int64_t   MVM_platform_free_memory(void);
int64_t   MVM_platform_total_memory(void);
MVMObject* MVM_platform_uname(MVMThreadContext *tc);
