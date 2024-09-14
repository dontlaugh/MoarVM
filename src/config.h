/* !!!
 * If you're looking at config.h and thinking of editing - this is a
 * generated file. See build/config.h.in, unless you're just twiddling
 * to get things to work on your platform before updating Configure,
 * in which case go right ahead. :-)
 * !!!
 */

/* Version information. */
#define MVM_VERSION "2024.08-30-gc5c876714"
#define MVM_VERSION_MAJOR 2024
#define MVM_VERSION_MINOR 08
#define MVM_VERSION_PATCH 30

/* Various compiler-specific pragmii. */
#define MVM_NO_RETURN 
#define MVM_NO_RETURN_ATTRIBUTE __attribute__((noreturn))
#define MVM_FORMAT(X, Y, Z) __attribute__((format(X, Y, Z)))
#define MVM_FALLTHROUGH __attribute__ ((fallthrough));

/* DLL interface definitions. */
#define MVM_DLL_IMPORT __attribute__ ((visibility ("default")))
#define MVM_DLL_EXPORT __attribute__ ((visibility ("default")))
#define MVM_DLL_LOCAL  __attribute__ ((visibility ("hidden")))

/* Set in case of big-endian sytems.
 * The default byte order is little-endian. */
#if 0
#define MVM_BIGENDIAN 0
#endif

/* pthread_yield() detection */
#if 0
#define MVM_HAS_PTHREAD_YIELD 0
#endif

/* pthread_setname_np() detection */
#if 1
#define MVM_HAS_PTHREAD_SETNAME_NP 1
#endif

#if 1
#define MVM_HAS_FN_MALLOC_TRIM 1
#endif

/* How this compiler does static inline functions. */
#define MVM_STATIC_INLINE static __inline__

#if 1
/* How this compiler declares thread local storage. */
#define MVM_THREAD_LOCAL _Thread_local
#endif

#if 1
#define MVM_CAN_UNALIGNED_INT32
#endif

#if 1
#define MVM_CAN_UNALIGNED_INT64
#endif

#if 1
#define MVM_CAN_UNALIGNED_NUM64
#endif

#define MVM_PTR_SIZE 8

#if 0
#define MVM_HAS_SUBSTANDARD_POW
#endif

#if 0
#define MVM_HAS_SUBSTANDARD_LOG
#endif

#if 0
#define MVM_HAS_SUBSTANDARD_LOG10
#endif

#if 0
#define MVM_HAS_SUBSTANDARD_ASIN
#endif

#if 0
#define MVM_HAS_SUBSTANDARD_ACOS
#endif

#if 1
#define MVM_BOOL _Bool
#endif

#if 1
#define MVM_HAS_RDTSCP
#endif

#if 1
#define MVM_HAS_ISINF
#endif

#if 1
#define MVM_HAS_ISNAN
#endif

#if 1
#define MVM_HAS_SIGNBIT
#endif

#if 1
#define MVM_USE_MIMALLOC
#endif

#if 1
#define MVM_USE_C11_ATOMICS
#endif

/* Should we translate \n to \r\n on output? */
#define MVM_TRANSLATE_NEWLINE_OUTPUT 0

#define MVM_JIT_ARCH MVM_JIT_ARCH_X64
#define MVM_JIT_PLATFORM MVM_JIT_PLATFORM_POSIX

#define MVM_VECTORIZE_LOOP 

/* Branch condition compiler hints */
#define MVM_LIKELY(condition) __builtin_expect(!!(condition), 1)
#define MVM_UNLIKELY(condition) __builtin_expect(!!(condition), 0)
#define MVM_EXPECT(condition, expection) __builtin_expect((condition), (expection))
