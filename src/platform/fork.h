#ifndef FORK_H
#define FORK_H
MVMint64 MVM_platform_supports_fork(MVMThreadContext *tc);
MVMint64 MVM_platform_fork(MVMThreadContext *tc);

#endif