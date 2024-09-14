struct MVMDLLRegistry {
    MVMString *hash_key;
    DLLib *lib;
    atomic_uintptr_t refcount;
};

int MVM_dll_load(struct MVMThreadContext *tc, MVMString *name, MVMString *path);
int MVM_dll_free(struct MVMThreadContext *tc, MVMString *name);
MVMObject * MVM_dll_find_symbol(struct MVMThreadContext *tc, MVMString *lib, MVMString *sym);
void MVM_dll_drop_symbol(struct MVMThreadContext *tc, MVMObject *obj);
