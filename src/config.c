/* !!!
 * If you're looking at config.c and thinking of editing - this is a
 * generated file. See build/config.c.in, unless you're just twiddling
 * to get things to work on your platform before updating Configure,
 * in which case go right ahead. :-)
 * !!!
 */

#include "moar.h"

#define add_entry(tc, hash, name, value) do { \
    MVMString * const key = MVM_string_ascii_decode_nt(tc, tc->instance->VMString, (name)); \
    MVMROOT(tc, key) { \
        MVMString * const value_str = MVM_string_ascii_decode_nt(tc, tc->instance->VMString, (value)); \
        MVMObject * const boxed_value = MVM_repr_box_str(tc, MVM_hll_current(tc)->str_box_type, value_str); \
        MVM_repr_bind_key_o(tc, hash, key, boxed_value); \
    } \
} while (0)

MVMObject *MVM_backend_config(struct MVMThreadContext *tc) {
    MVMObject *config = tc->instance->cached_backend_config;
    if (config)
        return config;

    config = MVM_repr_alloc_init(tc, tc->instance->boot_types.BOOTHash);
    MVMROOT(tc, config) {
        add_entry(tc, config, "ar", "ar");
        add_entry(tc, config, "arflags", "rcs");
        add_entry(tc, config, "arout", "");
        add_entry(tc, config, "as", "as");
        add_entry(tc, config, "asm", ".s");
        add_entry(tc, config, "asmout", "-o ");
        add_entry(tc, config, "asmswitch", "-S");
        add_entry(tc, config, "auxclean", "@:");
        add_entry(tc, config, "be", "0");
        add_entry(tc, config, "bindir", "/home/coleman/Code/MoarVM/install/bin");
        add_entry(tc, config, "booltype", "_Bool");
        add_entry(tc, config, "can_specific_werror", "1");
        add_entry(tc, config, "can_unaligned_int32", "1");
        add_entry(tc, config, "can_unaligned_int64", "1");
        add_entry(tc, config, "can_unaligned_num64", "1");
        add_entry(tc, config, "cancgoto", "1");
        add_entry(tc, config, "cat", "cat");
        add_entry(tc, config, "cc", "gcc");
        add_entry(tc, config, "ccdebugflags", "-g3");
        add_entry(tc, config, "ccdef", "-D");
        add_entry(tc, config, "ccdefflags", "-D_REENTRANT -D_FILE_OFFSET_BITS=64");
        add_entry(tc, config, "ccinc", "-I");
        add_entry(tc, config, "ccincsystem", "-isystem");
        add_entry(tc, config, "ccjitflags", "");
        add_entry(tc, config, "ccmiscflags", "-std=gnu99 -Wextra -Wall -Wno-unused-parameter -Wno-unused-function -Wno-missing-braces -Werror=pointer-arith -Werror=vla");
        add_entry(tc, config, "ccoptiflags", "-O3 -DNDEBUG");
        add_entry(tc, config, "ccout", "-o ");
        add_entry(tc, config, "ccshared", "-fPIC");
        add_entry(tc, config, "ccswitch", "-c");
        add_entry(tc, config, "ccwarnflags", "");
        add_entry(tc, config, "cflags", "-std=gnu99 -Wextra -Wall -Wno-unused-parameter -Wno-unused-function -Wno-missing-braces -Werror=pointer-arith -Werror=vla -O3 -DNDEBUG  -D_REENTRANT -D_FILE_OFFSET_BITS=64 -fPIC -DMVM_HEAPSNAPSHOT_FORMAT=2 -D_GNU_SOURCE");
        add_entry(tc, config, "cincludes", "");
        add_entry(tc, config, "cmpclean", "cd 3rdparty/cmp && $(RM) libcmp.a && $(RM) cmp.lib && $(RM) cmp.obj && $(MAKE) clean");
        add_entry(tc, config, "cmplib", "3rdparty/cmp/libcmp.a");
        add_entry(tc, config, "cmpobjects", "");
        add_entry(tc, config, "cmprule", "$(AR) $(ARFLAGS) $@ 3rdparty/cmp/*.o");
        add_entry(tc, config, "config", "");
        add_entry(tc, config, "cppout", "> ");
        add_entry(tc, config, "cppswitch", "-E");
        add_entry(tc, config, "crossconf", "");
        add_entry(tc, config, "dasm_flags", "-D POSIX=1");
        add_entry(tc, config, "dcbclean", "$(RM) 3rdparty/dyncall/dyncallback/libdyncallback_s.a");
        add_entry(tc, config, "dcblib", "3rdparty/dyncall/dyncallback/libdyncallback_s.a");
        add_entry(tc, config, "dcbobjects", "");
        add_entry(tc, config, "dcbrule", "@:");
        add_entry(tc, config, "dcclean", "cd 3rdparty/dyncall && $(MAKE) -f Makefile clean");
        add_entry(tc, config, "dclib", "3rdparty/dyncall/dyncall/libdyncall_s.a");
        add_entry(tc, config, "dcobjects", "");
        add_entry(tc, config, "dcrule", "cd 3rdparty/dyncall &&  ./configure && CC='$(CC)' CFLAGS='-fPIC' $(MAKE) -f Makefile ");
        add_entry(tc, config, "defs[0]", "_REENTRANT");
        add_entry(tc, config, "defs[1]", "_FILE_OFFSET_BITS=64");
        add_entry(tc, config, "dlclean", "$(RM) 3rdparty/dyncall/dynload/libdynload_s.a");
        add_entry(tc, config, "dll", "lib%s.so");
        add_entry(tc, config, "dllexport", "__attribute__ ((visibility (\"default\")))");
        add_entry(tc, config, "dllib", "3rdparty/dyncall/dynload/libdynload_s.a");
        add_entry(tc, config, "dllimport", "__attribute__ ((visibility (\"default\")))");
        add_entry(tc, config, "dlllocal", "__attribute__ ((visibility (\"hidden\")))");
        add_entry(tc, config, "dlobjects", "");
        add_entry(tc, config, "dlrule", "@:");
        add_entry(tc, config, "exe", "");
        add_entry(tc, config, "expect_condition", "__builtin_expect((condition), (expection))");
        add_entry(tc, config, "expect_likely", "__builtin_expect(!!(condition), 1)");
        add_entry(tc, config, "expect_unlikely", "__builtin_expect(!!(condition), 0)");
        add_entry(tc, config, "fallthrough", "__attribute__ ((fallthrough));");
        add_entry(tc, config, "formatattribute", "__attribute__((format(X, Y, Z)))");
        add_entry(tc, config, "has_booltype", "1");
        add_entry(tc, config, "has_fn_malloc_trim", "1");
        add_entry(tc, config, "has_isinf", "1");
        add_entry(tc, config, "has_isnan", "1");
        add_entry(tc, config, "has_pthread_setname_np", "1");
        add_entry(tc, config, "has_pthread_yield", "0");
        add_entry(tc, config, "has_rdtscp", "1");
        add_entry(tc, config, "has_signbit", "1");
        add_entry(tc, config, "has_stdatomic", "1");
        add_entry(tc, config, "has_substandard_acos", "0");
        add_entry(tc, config, "has_substandard_asin", "0");
        add_entry(tc, config, "has_substandard_log", "0");
        add_entry(tc, config, "has_substandard_log10", "0");
        add_entry(tc, config, "has_substandard_pow", "0");
        add_entry(tc, config, "has_thread_local", "1");
        add_entry(tc, config, "heapsnapformat", "2");
        add_entry(tc, config, "hllincludes", "moar libuv libtommath dyncall mimalloc");
        add_entry(tc, config, "impinst", "");
        add_entry(tc, config, "install", "	$(MKPATH) \"$(DESTDIR)$(PREFIX)/include/libuv\"\
	$(MKPATH) \"$(DESTDIR)$(PREFIX)/include/libuv/uv\"\
	$(CP) 3rdparty/libuv/include/*.h \"$(DESTDIR)$(PREFIX)/include/libuv\"\
	$(CP) 3rdparty/libuv/include/uv/*.h \"$(DESTDIR)$(PREFIX)/include/libuv/uv\"\
	$(MKPATH) \"$(DESTDIR)$(PREFIX)/include/libtommath\"\
	$(CP) 3rdparty/libtommath/*.h \"$(DESTDIR)$(PREFIX)/include/libtommath\"\
	$(MKPATH) \"$(DESTDIR)$(PREFIX)/include/dyncall\"\
	$(CP) 3rdparty/dyncall/dynload/*.h \"$(DESTDIR)$(PREFIX)/include/dyncall\"\
	$(CP) 3rdparty/dyncall/dyncall/*.h \"$(DESTDIR)$(PREFIX)/include/dyncall\"\
	$(CP) 3rdparty/dyncall/dyncallback/*.h \"$(DESTDIR)$(PREFIX)/include/dyncall\"\
	$(MKPATH) \"$(DESTDIR)$(PREFIX)/include/mimalloc\"\
	$(CP) 3rdparty/mimalloc/include/*.h \"$(DESTDIR)$(PREFIX)/include/mimalloc\"\
");
        add_entry(tc, config, "jit_arch", "MVM_JIT_ARCH_X64");
        add_entry(tc, config, "jit_obj", "$(JIT_OBJECTS) $(JIT_ARCH_X64)");
        add_entry(tc, config, "jit_platform", "MVM_JIT_PLATFORM_POSIX");
        add_entry(tc, config, "laoclean", "@:");
        add_entry(tc, config, "laolib", "__lao__");
        add_entry(tc, config, "laoobjects", "");
        add_entry(tc, config, "laorule", "@:");
        add_entry(tc, config, "ld", "gcc");
        add_entry(tc, config, "lddebugflags", "-g3");
        add_entry(tc, config, "lddir", "-L");
        add_entry(tc, config, "ldflags", " -O3 -DNDEBUG -Wl,-rpath,\"//home/coleman/Code/MoarVM/install/lib\"");
        add_entry(tc, config, "ldimp", "");
        add_entry(tc, config, "ldinstflags", "");
        add_entry(tc, config, "ldlibs", " -lm -lpthread -lrt -ldl");
        add_entry(tc, config, "ldmiscflags", "");
        add_entry(tc, config, "ldoptiflags", "-O3 -DNDEBUG");
        add_entry(tc, config, "ldout", "-o ");
        add_entry(tc, config, "ldrpath", "-Wl,-rpath,\"//home/coleman/Code/MoarVM/install/lib\"");
        add_entry(tc, config, "ldrpath_relocatable", "-Wl,-z,origin,-rpath,'$$ORIGIN/../lib'");
        add_entry(tc, config, "ldshared", "-shared -fPIC");
        add_entry(tc, config, "ldsys", "-l%s");
        add_entry(tc, config, "ldusr", "-l%s");
        add_entry(tc, config, "lib", "lib%s.a");
        add_entry(tc, config, "libdir", "/home/coleman/Code/MoarVM/install/lib");
        add_entry(tc, config, "lincludes", "");
        add_entry(tc, config, "mainflags", "-DMVM_SHARED");
        add_entry(tc, config, "mainlibs", "-L. -lmoar");
        add_entry(tc, config, "make", "make");
        add_entry(tc, config, "mastdir", "/home/coleman/Code/MoarVM/install/share/nqp/lib/MAST");
        add_entry(tc, config, "mimalloc_include", "-isystem3rdparty/mimalloc");
        add_entry(tc, config, "mimalloc_object", "3rdparty/mimalloc/src/static.o");
        add_entry(tc, config, "mkflags", "");
        add_entry(tc, config, "mknoisy", "ifneq ($(NOISY), 1)\
MSG = @echo\
CMD = @\
NOOUT = > /dev/null\
NOERR = 2> /dev/null\
endif\
");
        add_entry(tc, config, "moar", "libmoar.so");
        add_entry(tc, config, "moar_cincludes", " -I3rdparty/libuv/include -I3rdparty/libuv/src -I3rdparty/libtommath -I3rdparty/dyncall/dynload -I3rdparty/dyncall/dyncall -I3rdparty/dyncall/dyncallback -I3rdparty/mimalloc/include -I3rdparty/mimalloc/src");
        add_entry(tc, config, "moardll", "libmoar.so");
        add_entry(tc, config, "moarlib", "libmoar.a");
        add_entry(tc, config, "moarshared", "");
        add_entry(tc, config, "moarshared_norelocatable", "");
        add_entry(tc, config, "moarshared_relocatable", "");
        add_entry(tc, config, "name", "moar");
        add_entry(tc, config, "nativecall_backend", "dyncall");
        add_entry(tc, config, "noreturnattribute", "__attribute__((noreturn))");
        add_entry(tc, config, "noreturnspecifier", "");
        add_entry(tc, config, "nul", "/dev/null");
        add_entry(tc, config, "obj", ".o");
        add_entry(tc, config, "objflags", "-DMVM_BUILD_SHARED -fPIC");
        add_entry(tc, config, "objout", "-o ");
        add_entry(tc, config, "os", "linux");
        add_entry(tc, config, "osname", "linux");
        add_entry(tc, config, "osvers", "4.19.0");
        add_entry(tc, config, "perl", "/usr/bin/perl");
        add_entry(tc, config, "pkgconfig", "/usr/bin/pkg-config");
        add_entry(tc, config, "pkgconfig_works", "1");
        add_entry(tc, config, "platform", "$(PLATFORM_POSIX)");
        add_entry(tc, config, "prefix", "/home/coleman/Code/MoarVM/install");
        add_entry(tc, config, "ptr_size", "8");
        add_entry(tc, config, "rm", "rm -f");
        add_entry(tc, config, "sh", "/bin/sh");
        add_entry(tc, config, "shaclean", "$(RM) 3rdparty/sha1/libsha1.a 3rdparty/sha1/*.o");
        add_entry(tc, config, "shaincludedir", "3rdparty/sha1");
        add_entry(tc, config, "shalib", "3rdparty/sha1/libsha1.a");
        add_entry(tc, config, "shaobjects", "");
        add_entry(tc, config, "sharedlib", "");
        add_entry(tc, config, "sharule", "$(AR) $(ARFLAGS) $@ 3rdparty/sha1/*.o");
        add_entry(tc, config, "static_inline", "static __inline__");
        add_entry(tc, config, "staticlib", "");
        add_entry(tc, config, "syslibs[0]", "m");
        add_entry(tc, config, "syslibs[1]", "pthread");
        add_entry(tc, config, "syslibs[2]", "rt");
        add_entry(tc, config, "syslibs[3]", "dl");
        add_entry(tc, config, "thirdpartylibs", "3rdparty/cmp/libcmp.a 3rdparty/dyncall/dyncall/libdyncall_s.a 3rdparty/dyncall/dyncallback/libdyncallback_s.a 3rdparty/dyncall/dynload/libdynload_s.a 3rdparty/sha1/libsha1.a 3rdparty/libtommath/libtommath.a 3rdparty/libuv/libuv.a");
        add_entry(tc, config, "thread_local", "_Thread_local");
        add_entry(tc, config, "tomclean", "$(RM) 3rdparty/libtommath/libtommath.a 3rdparty/libtommath/*.o");
        add_entry(tc, config, "tomlib", "3rdparty/libtommath/libtommath.a");
        add_entry(tc, config, "tomobjects", "");
        add_entry(tc, config, "tomrule", "$(AR) $(ARFLAGS) $@ 3rdparty/libtommath/*.o");
        add_entry(tc, config, "translate_newline_output", "0");
        add_entry(tc, config, "use_c11_atomics", "1");
        add_entry(tc, config, "use_mimalloc", "1");
        add_entry(tc, config, "uvclean", "$(RM) 3rdparty/libuv/libuv.a $(UV_LINUX)");
        add_entry(tc, config, "uvlib", "3rdparty/libuv/libuv.a");
        add_entry(tc, config, "uvobjects", "$(UV_LINUX)");
        add_entry(tc, config, "uvrule", "$(AR) $(ARFLAGS) $@ $(UV_LINUX)");
        add_entry(tc, config, "vectorizerspecifier", "");
        add_entry(tc, config, "version", "2024.08-30-gc5c876714");
        add_entry(tc, config, "versionmajor", "2024");
        add_entry(tc, config, "versionminor", "08");
        add_entry(tc, config, "versionpatch", "30");

    }
    tc->instance->cached_backend_config = config;

    return config;
}
