/* Representation used for bootstrapping the KnowHOW type. */
#ifndef KNOWHOWREPR_H
#define KNOWHOWREPR_H

struct MVMKnowHOWREPRBody {
    /* Methods table; a hash. */
    MVMObject *methods;

    /* Array of attribute meta-objects. */
    MVMObject *attributes;

    /* Name of the type. */
    MVMString *name;
};
struct MVMKnowHOWREPR {
    MVMObject common;
    MVMKnowHOWREPRBody body;
};

/* Function for REPR setup. */
const MVMREPROps * MVMKnowHOWREPR_initialize(MVMThreadContext *tc);
#endif // KNOWHOWREPR_H
