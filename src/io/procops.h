#define MVM_PIPE_INHERIT        1
#define MVM_PIPE_IGNORE         2
#define MVM_PIPE_CAPTURE        4
#define MVM_PIPE_INHERIT_IN     1
#define MVM_PIPE_IGNORE_IN      2
#define MVM_PIPE_CAPTURE_IN     4
#define MVM_PIPE_INHERIT_OUT    8
#define MVM_PIPE_IGNORE_OUT    16
#define MVM_PIPE_CAPTURE_OUT   32
#define MVM_PIPE_INHERIT_ERR   64
#define MVM_PIPE_IGNORE_ERR   128
#define MVM_PIPE_CAPTURE_ERR  256
#define MVM_PIPE_MERGED_OUT_ERR 512

MVMObject * MVM_proc_getenvhash(struct MVMThreadContext *tc);
MVMObject * MVM_proc_spawn_async(struct MVMThreadContext *tc, MVMObject *queue, MVMString *prog,
         MVMObject *args, MVMString *cwd, MVMObject *env, MVMObject *callbacks);
void MVM_proc_kill_async(struct MVMThreadContext *tc, MVMObject *handle, int64_t signal);
int64_t MVM_proc_getpid(struct MVMThreadContext *tc);
int64_t MVM_proc_getppid(struct MVMThreadContext *tc);
int64_t MVM_proc_rand_i(struct MVMThreadContext *tc);
double MVM_proc_rand_n(struct MVMThreadContext *tc);
double MVM_proc_randscale_n(struct MVMThreadContext *tc, double scale);
void MVM_proc_seed(struct MVMThreadContext *tc, int64_t seed);
uint64_t MVM_proc_time(struct MVMThreadContext *tc);
MVMObject * MVM_proc_clargs(struct MVMThreadContext *tc);
MVMString * MVM_executable_name(struct MVMThreadContext *tc);
void MVM_proc_getrusage(struct MVMThreadContext *tc, MVMObject *result);
int64_t MVM_proc_fork(struct MVMThreadContext *tc);

#ifdef _WIN32
#include <wchar.h>
 char ** MVM_UnicodeToUTF8_argv(const int argc, wchar_t **argv);
#endif

