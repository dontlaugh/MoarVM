/* Representation used by VM-level hashes. */
#ifndef MVMHASH_H
#define MVMHASH_H

struct MVMHashEntry {
    /* hash handle inline struct, including the key. */
    struct MVMStrHashHandle hash_handle;
    /* value object */
    MVMObject *value;
};

struct MVMHashBody {
    MVMStrHashTable hashtable;
};
struct MVMHash {
    MVMObject common;
    MVMHashBody body;
};

/* Function for REPR setup. */
const MVMREPROps * MVMHash_initialize(MVMThreadContext *tc);

void MVMHash_at_key(MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data, MVMObject *key_obj, MVMRegister *result, MVMuint16 kind);
void MVMHash_bind_key(MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data, MVMObject *key_obj, MVMRegister value, MVMuint16 kind);
#endif // MVMHASH_H
