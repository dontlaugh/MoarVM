#include "moar.h"

int64_t MVM_platform_supports_fork(struct MVMThreadContext *tc) {
    return 0;
}

/* Windows does not support fork() */
int64_t MVM_platform_fork(struct MVMThreadContext *tc) {
    MVM_exception_throw_adhoc(tc, "This platform does not support fork()");
}
