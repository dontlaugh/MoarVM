/* A Tracked wraps a value (obj/str/int/num) such that we can . */
#ifndef MVMTRACKED_H
#define MVMTRACKED_H

struct MVMTrackedBody {
    MVMRegister value;
    MVMCallsiteFlags kind;
};
struct MVMTracked {
    MVMObject common;
    MVMTrackedBody body;
};

/* Function for REPR setup. */
const MVMREPROps * MVMTracked_initialize(MVMThreadContext *tc);

/* Operations for creating MVMTracked objects. */
MVMObject * MVM_tracked_create(MVMThreadContext *tc, MVMRegister value, MVMCallsiteFlags kind);
#endif // MVMTRACKED_H
