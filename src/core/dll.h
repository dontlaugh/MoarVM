#ifndef DLL_H
#define DLL_H

struct MVMDLLRegistry {
    MVMString *hash_key;
    DLLib *lib;
    AO_t refcount;
};

int MVM_dll_load(MVMThreadContext *tc, MVMString *name, MVMString *path);
int MVM_dll_free(MVMThreadContext *tc, MVMString *name);
MVMObject * MVM_dll_find_symbol(MVMThreadContext *tc, MVMString *lib, MVMString *sym);
void MVM_dll_drop_symbol(MVMThreadContext *tc, MVMObject *obj);
#endif // DLL_H
