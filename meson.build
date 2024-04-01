project('MoarVM', 'c')

cmake = import('cmake')

dyncall_subproject = cmake.subproject('dyncall')
dyncall_s_lib = dyncall_subproject.dependency('dyncall_s')
shared_library('dyncall_s', dependencies : [dyncall_s_lib])
shared_library('dyncallback_s', dependencies : [dyncall_s_lib])
shared_library('dynload_s', dependencies : [dyncall_s_lib])

mimalloc_subproject = cmake.subproject('mimalloc')
mimalloc_lib = mimalloc_subproject.dependency('mimalloc')
shared_library('mimalloc', dependencies : [mimalloc_lib])
static_library('mimalloc-static', dependencies : [mimalloc_lib])

# Note: we could use WrapDB package management for libuv;
libuv_opts = cmake.subproject_options()
# libuv_opts.add_cmake_defines({'LIBUV_BUILD_SHARED': true})
libuv_subproject = cmake.subproject('libuv', options : libuv_opts)
message('CMake targets:\n - ' + '\n - '.join(libuv_subproject.target_list()))
# if set LIBUV_BUILD_SHARED we can build a shared lib
# libuv_lib = mimalloc_subproject.dependency('uv')
# shared_library('libuv', dependencies : [libuv_lib])
libuv_lib_a = libuv_subproject.dependency('uv_a')
static_library('uv_a', dependencies : [libuv_lib_a])

# Latest master of ryu has CMake support
ryu_subproject = cmake.subproject('ryu')
ryu_lib = ryu_subproject.dependency('ryu')
static_library('ryu', dependencies : [ryu_lib])

#
# Generate: src/gen/config.c, src/gen/config.h
#
# Todo: use Meson platform test conditionals where needed (e.g. endianness)
conf_data = configuration_data()
# Version
conf_data.set('version', '2024.03')
conf_data.set('versionmajor', '2024')
conf_data.set('versionminor', '03')
conf_data.set('versionpatch', '0')
# Compiler pragmas
conf_data.set('noreturnspecifier', '')
conf_data.set('noreturnattribute', '')
conf_data.set('formatattribute', '')
conf_data.set('fallthrough', '')
# DLL interface definitions
conf_data.set('dllimport', '')
conf_data.set('dllexport', '')
conf_data.set('dlllocal', '')

# Big endian?
conf_data.set('be', '0')

# pthread_yield() detection
conf_data.set('has_pthread_yield', '0')

conf_data.set('has_pthread_setname_np', '1')

conf_data.set('has_fn_malloc_trim', '1')

# How this compiler does static inline functions
conf_data.set('static_inline', '__inline__')

# Whether (and how) this compiler does thread local storage
conf_data.set('has_thread_local', '1')
conf_data.set('thread_local', '_Thread_local')


conf_data.set('can_unaligned_int32', '1')
conf_data.set('can_unaligned_int64', '1')
conf_data.set('can_unaligned_num64', '1')

conf_data.set('ptr_size', '8')

conf_data.set('has_substandard_pow', '0')
conf_data.set('has_substandard_log', '0')
conf_data.set('has_substandard_log10', '0')
conf_data.set('has_substandard_asin', '0')
conf_data.set('has_substandard_acos', '0')

conf_data.set('has_booltype', '1')

conf_data.set('has_rdtscp', '1')

conf_data.set('has_isinf', '1')


conf_data.set('has_isnan', '1')

conf_data.set('has_signbit', '1')

conf_data.set('use_mimalloc', '1')

conf_data.set('use_c11_atomics', '1')

# Translate \n to \r\n?
conf_data.set('translate_newline_output', '0')

conf_data.set('jit_arch', 'MVM_JIT_ARCH_X64')

conf_data.set('jit_platform', 'MVM_JIT_PLATFORM_POSIX')

conf_data.set('vectorizerspecifier', '')

# Branch condition compiler hints
conf_data.set('expect_likely', '__builtin_expect(!!(condition), 1)')
conf_data.set('expect_unlikely', '__builtin_expect(!!(condition), 0)')
conf_data.set('expect_condition', '__builtin_expect((condition), (expection))')


subdir('src')
