/* Representation used by Uninstantiables. */
#ifndef UNINSTANTIABLE_H
#define UNINSTANTIABLE_H

struct MVMUninstantiable {
    MVMObject common;
};

/* Function for REPR setup. */
const MVMREPROps * MVMUninstantiable_initialize(MVMThreadContext *tc);
#endif // UNINSTANTIABLE_H
