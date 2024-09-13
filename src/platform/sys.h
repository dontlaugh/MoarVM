/* Tries to determine the number of logical CPUs available to the process.
 * May return 0 on error.
 */
#ifndef SYS_H
#define SYS_H
MVMint64   MVM_platform_cpu_count(void);
MVMint64   MVM_platform_free_memory(void);
MVMint64   MVM_platform_total_memory(void);
MVMObject* MVM_platform_uname(MVMThreadContext *tc);
#endif