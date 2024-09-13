/* Representation used by HashAttrStore. */
#ifndef HASHATTRSTORE_H
#define HASHATTRSTORE_H

struct MVMHashAttrStoreBody {
    MVMStrHashTable hashtable;
};
struct MVMHashAttrStore {
    MVMObject common;
    MVMHashAttrStoreBody body;
};

/* Function for REPR setup. */
const MVMREPROps * MVMHashAttrStore_initialize(MVMThreadContext *tc);
#endif // HASHATTRSTORE_H
