/* Representation for C Str. */
#ifndef CSTR_H
#define CSTR_H

struct MVMCStrBody {
    MVMString *orig;
    char      *cstr;
};

struct MVMCStr {
    MVMObject common;
    MVMCStrBody body;
};

/* Initializes the CStr REPR. */
const MVMREPROps * MVMCStr_initialize(MVMThreadContext *tc);
#endif // CSTR_H
