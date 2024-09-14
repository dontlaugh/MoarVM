#if defined _WIN32
#define MVM_platform_thread_yield SwitchToThread
#elif defined MVM_HAS_PTHREAD_YIELD
#include <pthread.h>
#define MVM_platform_thread_yield pthread_yield
#else
#include <sched.h>
#define MVM_platform_thread_yield sched_yield
#endif

#if defined _WIN32
#define MVM_platform_thread_exit(status) ExitThread(0)
#define MVM_platform_thread_id() (int64_t)GetCurrentThreadId()
#if defined MVM_HAS_PTHREAD_SETNAME_NP
#include <pthread.h>
#endif
#else
#define MVM_platform_thread_exit(status) pthread_exit(status)
#define MVM_platform_thread_id() (int64_t)uv_thread_self()
#endif
