struct MVMDLLSymBody {
    void *address;
    MVMDLLRegistry *dll;
};

struct MVMDLLSym {
    MVMObject common;
    MVMDLLSymBody body;
};

const MVMREPROps * MVMDLLSym_initialize(struct MVMThreadContext *tc);
