#include <dlfcn.h>

typedef void DLLib;

ffi_type * MVM_nativecall_get_ffi_type(struct MVMThreadContext *tc, uint64_t type_id);
ffi_abi MVM_nativecall_get_calling_convention(struct MVMThreadContext *tc, MVMString *name);
#define MVM_nativecall_load_lib(path)       dlopen(path, RTLD_NOW|RTLD_GLOBAL)
#define MVM_nativecall_free_lib(lib)        do { if(lib) dlclose(lib); } while (0)
#define MVM_nativecall_find_sym(lib, name)  dlsym(lib, name)
