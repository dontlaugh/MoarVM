project('moar', 'c',
  version: '2025.12',
  license: 'Artistic-2.0',
  meson_version: '>=1.0.0',
  default_options: [
    'c_std=c99',
    'warning_level=1',
    'buildtype=debugoptimized',
  ]
)

# Parse version components
version_parts = meson.project_version().split('.')
version_major = version_parts[0].to_int()
version_minor = version_parts[1].to_int()

# Detect host system
host_system = host_machine.system()
host_cpu = host_machine.cpu_family()
is_windows = host_system == 'windows'
is_darwin = host_system == 'darwin'
is_linux = host_system == 'linux'
is_bsd = host_system.contains('bsd') or host_system == 'dragonfly'

# Detect compiler
cc = meson.get_compiler('c')
is_msvc = cc.get_id() == 'msvc'
is_gcc = cc.get_id() == 'gcc'
is_clang = cc.get_id() == 'clang'

# Build options
opt_jit = get_option('jit')
opt_static = get_option('static')
opt_coverage = get_option('coverage')
opt_relocatable = get_option('relocatable')
opt_asan = get_option('asan')
opt_ubsan = get_option('ubsan')
opt_tsan = get_option('tsan')
opt_valgrind = get_option('valgrind')
opt_dtrace = get_option('dtrace')
opt_telemeh = get_option('telemeh')
opt_mimalloc = get_option('mimalloc')
opt_c11_atomics = get_option('c11-atomics')
opt_use_libffi = get_option('use-libffi')
opt_show_autovect = get_option('show-autovect')

# Configuration data
cdata = configuration_data()
cdata.set_quoted('MVM_VERSION', meson.project_version())
cdata.set('MVM_VERSION_MAJOR', version_major)
cdata.set('MVM_VERSION_MINOR', version_minor)
cdata.set('MVM_VERSION_PATCH', 0)

# ============================================================================
# Feature Detection
# ============================================================================

# Compiler attributes and specifiers
if is_msvc
  cdata.set('noreturnspecifier', '__declspec(noreturn)')
  cdata.set('noreturnattribute', '')
  cdata.set('formatattribute', '')
  cdata.set('fallthrough', '')
  cdata.set('dllimport', '__declspec(dllimport)')
  cdata.set('dllexport', '__declspec(dllexport)')
  cdata.set('dlllocal', '')
else
  if cc.has_function_attribute('noreturn')
    cdata.set('noreturnspecifier', '')
    cdata.set('noreturnattribute', '__attribute__((noreturn))')
  else
    cdata.set('noreturnspecifier', '')
    cdata.set('noreturnattribute', '')
  endif

  if cc.has_function_attribute('format')
    cdata.set('formatattribute', '__attribute__((format(X, Y, Z)))')
  else
    cdata.set('formatattribute', '')
  endif

  # Fallthrough attribute (C23/GCC 7+/Clang)
  if cc.compiles('''
    int main(void) {
      int x = 1;
      switch(x) {
        case 1:
          __attribute__((fallthrough));
        case 2:
          break;
      }
      return 0;
    }
    ''', name: 'fallthrough attribute')
    cdata.set('fallthrough', '__attribute__((fallthrough))')
  else
    cdata.set('fallthrough', '')
  endif

  # DLL visibility attributes
  if cc.has_function_attribute('visibility')
    cdata.set('dllimport', '__attribute__((visibility("default")))')
    cdata.set('dllexport', '__attribute__((visibility("default")))')
    cdata.set('dlllocal', '__attribute__((visibility("hidden")))')
  else
    cdata.set('dllimport', '')
    cdata.set('dllexport', '')
    cdata.set('dlllocal', '')
  endif
endif

# Static inline
static_inline_code = '''
  #include <stdlib.h>
  static inline int foo(void) { return 0; }
  int main(void) { return foo(); }
'''
if cc.compiles(static_inline_code, name: 'static inline')
  cdata.set('static_inline', 'static inline')
else
  cdata.set('static_inline', 'static')
endif

# Thread local storage
has_thread_local = false
thread_local_specifier = ''

if is_msvc
  has_thread_local = true
  thread_local_specifier = '__declspec(thread)'
else
  # Try C11 _Thread_local
  if cc.compiles('_Thread_local int x; int main(void) { return 0; }', name: '_Thread_local')
    has_thread_local = true
    thread_local_specifier = '_Thread_local'
  # Try __thread (GCC/Clang extension)
  elif cc.compiles('__thread int x; int main(void) { return 0; }', name: '__thread')
    has_thread_local = true
    thread_local_specifier = '__thread'
  endif
endif

cdata.set('has_thread_local', has_thread_local ? 1 : 0)
if has_thread_local
  cdata.set('thread_local', thread_local_specifier)
endif

# Pointer size
cdata.set('ptr_size', cc.sizeof('void *'))

# Endianness
if host_machine.endian() == 'big'
  cdata.set('be', 1)
else
  cdata.set('be', 0)
endif

# Unaligned access support
can_unaligned_int32 = cc.compiles('''
  #include <stdint.h>
  #include <stdlib.h>
  int main(void) {
    char buf[8];
    uint32_t *p = (uint32_t *)(buf + 1);
    *p = 0x12345678;
    return EXIT_SUCCESS;
  }
  ''', name: 'unaligned int32 access')
cdata.set('can_unaligned_int32', can_unaligned_int32 ? 1 : 0)

can_unaligned_int64 = cc.compiles('''
  #include <stdint.h>
  #include <stdlib.h>
  int main(void) {
    char buf[16];
    uint64_t *p = (uint64_t *)(buf + 1);
    *p = 0x123456789ABCDEF0ULL;
    return EXIT_SUCCESS;
  }
  ''', name: 'unaligned int64 access')
cdata.set('can_unaligned_int64', can_unaligned_int64 ? 1 : 0)

can_unaligned_num64 = cc.compiles('''
  #include <stdlib.h>
  int main(void) {
    char buf[16];
    double *p = (double *)(buf + 1);
    *p = 1.23456789;
    return EXIT_SUCCESS;
  }
  ''', name: 'unaligned double access')
cdata.set('can_unaligned_num64', can_unaligned_num64 ? 1 : 0)

# Math functions
has_isinf = cc.has_function('isinf', prefix: '#include <math.h>')
has_isnan = cc.has_function('isnan', prefix: '#include <math.h>')
has_signbit = cc.has_function('signbit', prefix: '#include <math.h>')
cdata.set('has_isinf', has_isinf ? 1 : 0)
cdata.set('has_isnan', has_isnan ? 1 : 0)
cdata.set('has_signbit', has_signbit ? 1 : 0)

# Substandard math functions (some platforms have broken implementations)
# We'll assume they're good by default; specific platforms can override
cdata.set('has_substandard_pow', 0)
cdata.set('has_substandard_log', 0)
cdata.set('has_substandard_log10', 0)
cdata.set('has_substandard_asin', 0)
cdata.set('has_substandard_acos', 0)

# C bool type
has_booltype = cc.compiles('''
  #include <stdbool.h>
  int main(void) { bool b = true; return b ? 0 : 1; }
  ''', name: 'stdbool.h bool type')
cdata.set('has_booltype', has_booltype ? 1 : 0)
if has_booltype
  cdata.set('booltype', '_Bool')
endif

# pthread functions
has_pthread_yield = cc.has_function('pthread_yield',
  prefix: '#include <pthread.h>',
  dependencies: thread_dep)
cdata.set('has_pthread_yield', has_pthread_yield ? 1 : 0)

has_pthread_setname_np = cc.has_function('pthread_setname_np',
  prefix: '#include <pthread.h>',
  dependencies: thread_dep)
cdata.set('has_pthread_setname_np', has_pthread_setname_np ? 1 : 0)

# malloc_trim (GNU extension)
has_fn_malloc_trim = cc.has_function('malloc_trim',
  prefix: '#include <malloc.h>')
cdata.set('has_fn_malloc_trim', has_fn_malloc_trim ? 1 : 0)

# RDTSCP instruction (x86/x64)
has_rdtscp = false
if host_cpu in ['x86', 'x86_64']
  has_rdtscp = cc.compiles('''
    #include <stdint.h>
    int main(void) {
      uint32_t aux;
      __builtin_ia32_rdtscp(&aux);
      return 0;
    }
    ''', name: 'RDTSCP support')
endif
cdata.set('has_rdtscp', has_rdtscp ? 1 : 0)

# C11 atomics (stdatomic.h)
has_stdatomic = cc.check_header('stdatomic.h') and cc.compiles('''
  #include <stdatomic.h>
  int main(void) {
    atomic_int x = ATOMIC_VAR_INIT(0);
    atomic_fetch_add(&x, 1);
    return atomic_load(&x);
  }
  ''', name: 'working stdatomic.h')

# Determine if we use C11 atomics
use_c11_atomics = false
if opt_c11_atomics.enabled() or (opt_c11_atomics.auto() and has_stdatomic)
  use_c11_atomics = true
endif
cdata.set('use_c11_atomics', use_c11_atomics ? 1 : 0)

# Computed goto (GCC/Clang extension)
can_cgoto = cc.compiles('''
  int main(void) {
    void *labels[] = {&&label1, &&label2};
    goto *labels[0];
    label1: return 1;
    label2: return 2;
  }
  ''', name: 'computed goto')

# Compiler optimization hints
if cc.has_function_attribute('expect')
  cdata.set('expect_likely', '__builtin_expect((condition), 1)')
  cdata.set('expect_unlikely', '__builtin_expect((condition), 0)')
  cdata.set('expect_condition', '__builtin_expect((condition), (expection))')
else
  cdata.set('expect_likely', '(condition)')
  cdata.set('expect_unlikely', '(condition)')
  cdata.set('expect_condition', '(condition)')
endif

# Vectorization specifier
vectorizer_specifier = ''
if is_gcc or is_clang
  if cc.compiles('void f(void) __attribute__((optimize("-ftree-vectorize"))); void f(void) {} int main(void) { return 0; }',
                  name: 'vectorize attribute')
    vectorizer_specifier = '__attribute__((optimize("-ftree-vectorize")))'
  endif
endif
cdata.set('vectorizerspecifier', vectorizer_specifier)

# Platform-specific settings
cdata.set('translate_newline_output', is_windows ? 1 : 0)

# PTY support (will be refined with libuv detection)
cdata.set('has_libuv_pty', 0)
cdata.set('do_pty_ourself', 0)

# Mimalloc decision (will be set based on option and stdatomic)
use_mimalloc = false
if opt_mimalloc.enabled() or (opt_mimalloc.auto() and (has_stdatomic or is_msvc))
  use_mimalloc = true
endif
cdata.set('use_mimalloc', use_mimalloc ? 1 : 0)

# JIT configuration (architecture and platform)
jit_enabled = false
jit_arch = 0
jit_platform = 0

if opt_jit.enabled() or (opt_jit.auto() and host_cpu == 'x86_64')
  if host_cpu == 'x86_64'
    jit_enabled = true
    jit_arch = 1  # X64
    if is_windows
      jit_platform = 1  # WIN32
    else
      jit_platform = 2  # POSIX
    endif
  else
    if opt_jit.enabled()
      warning('JIT requested but not available for CPU architecture: ' + host_cpu)
    endif
  endif
endif

cdata.set('jit_arch', jit_arch)
cdata.set('jit_platform', jit_platform)

# Compiler flags
add_project_arguments(cc.get_supported_arguments([
  '-D_REENTRANT',
  '-D_FILE_OFFSET_BITS=64',
]), language: 'c')

# Platform-specific defines
if is_windows
  add_project_arguments([
    '-DWIN32',
    '-D_WIN32_WINNT=0x0600',  # Windows Vista+
  ], language: 'c')
endif

# Warning flags
warning_flags = []
if not is_msvc
  warning_flags += [
    '-Wall',
    '-Wextra',
    '-Wno-unused-parameter',
    '-Wno-unused-function',
  ]
endif
add_project_arguments(cc.get_supported_arguments(warning_flags), language: 'c')

# Sanitizer flags
sanitizer_args = []
sanitizer_link_args = []

if opt_asan
  sanitizer_args += ['-fsanitize=address']
  sanitizer_link_args += ['-fsanitize=address']
endif

if opt_ubsan
  sanitizer_args += ['-fsanitize=undefined']
  sanitizer_link_args += ['-fsanitize=undefined']
endif

if opt_tsan
  sanitizer_args += ['-fsanitize=thread']
  sanitizer_link_args += ['-fsanitize=thread']
endif

if sanitizer_args.length() > 0
  add_project_arguments(sanitizer_args, language: 'c')
  add_project_link_arguments(sanitizer_link_args, language: 'c')
endif

# Coverage flags
if opt_coverage
  if is_gcc or is_clang
    add_project_arguments(['-fprofile-arcs', '-ftest-coverage'], language: 'c')
    add_project_link_arguments(['-fprofile-arcs', '-ftest-coverage'], language: 'c')
  endif
endif

# Auto-vectorization reporting
if opt_show_autovect
  if is_gcc
    add_project_arguments(['-ftree-vectorize', '-fopt-info-vec'], language: 'c')
  elif is_clang
    add_project_arguments(['-Rpass=loop-vectorize', '-Rpass-analysis=loop-vectorize'], language: 'c')
  endif
endif

# Thread support (required)
thread_dep = dependency('threads')

# Installation directories
bindir = get_option('bindir')
libdir = get_option('libdir')
mastdir = get_option('mastdir')
if mastdir == ''
  mastdir = libdir / 'moar' / 'MAST'
endif

# Summary
message('MoarVM Configuration:')
message('  Version:          ' + meson.project_version())
message('  Host system:      ' + host_system)
message('  Host CPU:         ' + host_cpu)
message('  Compiler:         ' + cc.get_id() + ' ' + cc.version())
message('  JIT:              ' + opt_jit.to_string())
message('  Static library:   ' + opt_static.to_string())
message('  Sanitizers:       ASAN=' + opt_asan.to_string() +
                            ' UBSAN=' + opt_ubsan.to_string() +
                            ' TSAN=' + opt_tsan.to_string())

# ============================================================================
# Configuration File Generation
# ============================================================================

# Generate config.h
config_h = configure_file(
  input: 'build/config.h.in',
  output: 'config.h',
  configuration: cdata,
  install: false
)

# Prepare backend configuration data for config.c
# This will be a simple key-value structure
backend_config_entries = []
backend_config_entries += 'add_entry(tc, config, "version", "@0@");'.format(meson.project_version())
backend_config_entries += 'add_entry(tc, config, "versionmajor", "@0@");'.format(version_major)
backend_config_entries += 'add_entry(tc, config, "versionminor", "@0@");'.format(version_minor)
backend_config_entries += 'add_entry(tc, config, "osname", "@0@");'.format(host_system)
backend_config_entries += 'add_entry(tc, config, "archname", "@0@");'.format(host_cpu)
backend_config_entries += 'add_entry(tc, config, "jit", "@0@");'.format(jit_enabled ? '1' : '0')
backend_config_entries += 'add_entry(tc, config, "staticlib", "@0@");'.format(opt_static ? '1' : '0')

backend_config_str = '\n        '.join(backend_config_entries)

# Create configuration data for config.c
config_c_data = configuration_data()
config_c_data.set('backendconfig', backend_config_str)

# Generate config.c
config_c = configure_file(
  input: 'build/config.c.in',
  output: 'config.c',
  configuration: config_c_data,
  install: false
)

# ============================================================================
# Dependencies
# ============================================================================

# Vendored dependencies (always built from 3rdparty/)
subdir('3rdparty')

# 3rd party library dependencies
# These can be satisfied by system libraries or subprojects

# LibUV (async I/O)
if get_option('use-system-libuv')
  libuv_dep = dependency('libuv', required: true)
else
  libuv_dep = dependency('libuv', fallback: ['libuv', 'libuv_dep'], required: true)
endif

# Libtommath (big integer math)
if get_option('use-system-libtommath')
  libtommath_dep = dependency('libtommath', required: true)
else
  libtommath_dep = dependency('libtommath',
    fallback: ['libtommath', 'libtommath_dep'],
    required: true)
endif

# Atomic operations
if use_c11_atomics
  # Using C11 atomics, no library needed
  atomic_ops_dep = declare_dependency()
else
  if get_option('use-system-libatomic-ops')
    atomic_ops_dep = dependency('atomic_ops', required: true)
  else
    atomic_ops_dep = dependency('atomic_ops',
      fallback: ['libatomic_ops', 'atomic_ops_dep'],
      required: true)
  endif
endif

# Native call backend: dyncall or libffi
if opt_use_libffi
  # Use libffi (system library)
  libffi_dep = dependency('libffi', required: true)
  nativecall_dep = libffi_dep
  nativecall_backend = 'libffi'
else
  # Use dyncall (bundled or system)
  if get_option('use-system-dyncall')
    dyncall_dep = dependency('dyncall', required: true)
  else
    dyncall_dep = dependency('dyncall',
      fallback: ['dyncall', 'dyncall_dep'],
      required: true)
  endif
  nativecall_dep = dyncall_dep
  nativecall_backend = 'dyncall'
endif

# CMP (MessagePack implementation)
cmp_dep = dependency('cmp', fallback: ['cmp', 'cmp_dep'], required: true)

# Ryu (float-to-string conversion)
ryu_dep = dependency('ryu', fallback: ['ryu', 'ryu_dep'], required: true)

# DynASM (JIT dynamic assembler) - only needed if JIT is enabled
if jit_enabled
  dynasm_dep = dependency('dynasm', fallback: ['dynasm', 'dynasm_dep'], required: true)
endif

# RapidHash (hash function library)
rapidhash_dep = dependency('rapidhash', fallback: ['rapidhash', 'rapidhash_dep'], required: true)

# Mimalloc (optional, high-performance allocator)
if use_mimalloc
  if get_option('use-system-mimalloc')
    mimalloc_dep = dependency('mimalloc', required: true)
  else
    mimalloc_dep = dependency('mimalloc',
      fallback: ['mimalloc', 'mimalloc_dep'],
      required: true)
  endif
else
  mimalloc_dep = declare_dependency()
endif

# Collect all dependencies
moar_deps = [
  thread_dep,
  libuv_dep,
  libtommath_dep,
  atomic_ops_dep,
  nativecall_dep,
  cmp_dep,
  ryu_dep,
  rapidhash_dep,
  mimalloc_dep,
  sha1_dep,
  freebsd_dep,
]

if is_windows
  # Add msinttypes for old MSVC
  moar_deps += msinttypes_dep
endif

if jit_enabled
  moar_deps += dynasm_dep
endif

# Math library (needed on some platforms)
m_dep = cc.find_library('m', required: false)
if m_dep.found()
  moar_deps += m_dep
endif

# Subsequent phases will add:
# - Code generation (Phase 3)
# - Source compilation (Phase 4)
# - Installation rules (Phase 6)
