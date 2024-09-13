/* Representation holding a pointer to a C function, which is passed
 * MVMArgs describing the arguments that it receives. */
#ifndef MVMCFUNCTION_H
#define MVMCFUNCTION_H

struct MVMCFunctionBody {
    void (*func) (MVMThreadContext *tc, MVMArgs arg_info);
};
struct MVMCFunction {
    MVMObject common;
    MVMCFunctionBody body;
};

/* Function for REPR setup. */
const MVMREPROps * MVMCFunction_initialize(MVMThreadContext *tc);
#endif // MVMCFUNCTION_H
