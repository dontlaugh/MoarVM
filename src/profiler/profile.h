#ifndef PROFILE_H
#define PROFILE_H

void MVM_profile_start(MVMThreadContext *tc, MVMObject *config);
MVMObject * MVM_profile_end(MVMThreadContext *tc);
#endif // PROFILE_H
