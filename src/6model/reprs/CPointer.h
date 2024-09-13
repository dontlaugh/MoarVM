/* Representation for C Pointer. */
#ifndef CPOINTER_H
#define CPOINTER_H

struct MVMCPointerBody {
    void *ptr;
};

struct MVMCPointer {
    MVMObject common;
    MVMCPointerBody body;
};

/* Initializes the CPointer REPR. */
const MVMREPROps * MVMCPointer_initialize(MVMThreadContext *tc);
#endif // CPOINTER_H
