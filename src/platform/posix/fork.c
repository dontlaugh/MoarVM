#include "moar.h"
#include <unistd.h>

int64_t MVM_platform_supports_fork(MVMThreadContext *tc) {
    return 1;
}

int64_t MVM_platform_fork(MVMThreadContext *tc) {
    return fork();
}
