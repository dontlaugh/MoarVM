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

# Filesystem module
fs = import('fs')

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

# ============================================================================
# Code Generation
# ============================================================================

# TODO: Port these Perl/Raku scripts to Python for better Meson integration
# For now, we wrap the existing scripts as an incremental migration step

# Find Perl and Raku interpreters
perl = find_program('perl', required: true)
# Raku is optional - only needed for update_ops which we'll handle separately
raku = find_program('raku', 'perl6', required: false)

# JIT code generation (only if JIT is enabled)
jit_gen_files = []

if jit_enabled
  # Expression template compiler for JIT
  # Generates src/jit/core_templates.h from .expr files
  expr_files = files(
    'src/jit/core_templates.expr',
    'src/jit/macro.expr',
    'src/jit/unsafe.expr',
  )

  core_templates_h = custom_target('core_templates.h',
    input: expr_files,
    output: 'core_templates.h',
    command: [
      perl,
      '-I@SOURCE_ROOT@/tools/lib',
      '@SOURCE_ROOT@/tools/expr-template-compiler.pl',
      '-o', '@OUTPUT@',
      '@INPUT0@',  # core_templates.expr (main input)
    ],
    depend_files: files(
      'tools/lib/sexpr.pm',
      'tools/lib/expr_ops.pm',
      'tools/lib/oplist.pm',
      'src/jit/expr_ops.h',
    ),
  )
  jit_gen_files += core_templates_h

  # Tile table generator for JIT (x64 specific)
  if host_cpu == 'x86_64'
    tile_pattern_h = custom_target('tile_pattern.h',
      input: 'src/jit/x64/tile_pattern.tile',
      output: 'tile_pattern.h',
      command: [
        perl,
        '-I@SOURCE_ROOT@/tools/lib',
        '@SOURCE_ROOT@/tools/tiler-table-generator.pl',
        '-o', '@OUTPUT@',
        '@INPUT@',
      ],
      depend_files: files(
        'tools/lib/sexpr.pm',
        'tools/lib/expr_ops.pm',
        'tools/lib/oplist.pm',
      ),
    )
    jit_gen_files += tile_pattern_h
  endif
endif

# Op code generation
# TODO: This requires Raku. For now, we'll skip automatic regeneration
# and rely on pre-generated files checked into the repository.
# When we port to Python, this will be integrated properly.
if raku.found()
  message('Raku found - op code regeneration available')
  # Note: update_ops.raku is typically run manually, not during build
else
  message('Raku not found - using pre-generated op code files')
  message('Run tools/update_ops.raku manually if you modify src/core/oplist')
endif

# ============================================================================
# Pkg-config File Generation
# ============================================================================

# This will be completed once we have the library target in Phase 4
# For now, prepare the module
pkg = import('pkgconfig')

# ============================================================================
# Source Files
# ============================================================================

# Include directories
moar_inc = include_directories('src', 'src/gen')

# Core library sources (organized by directory)
core_sources = files(
  'src/core/callsite.c',
  'src/core/args.c',
  'src/core/exceptions.c',
  'src/core/interp.c',
  'src/core/str_hash_table.c',
  'src/core/fixkey_hash_table.c',
  'src/core/index_hash_table.c',
  'src/core/ptr_hash_table.c',
  'src/core/uni_hash_table.c',
  'src/core/threadcontext.c',
  'src/core/compunit.c',
  'src/core/bytecode.c',
  'src/core/frame.c',
  'src/core/callstack.c',
  'src/core/validation.c',
  'src/core/bytecodedump.c',
  'src/core/threads.c',
  'src/core/ops.c',
  'src/core/hll.c',
  'src/core/loadbytecode.c',
  'src/core/coerce.c',
  'src/core/dll.c',
  'src/core/ext.c',
  'src/core/nativecall.c',
  'src/core/continuation.c',
  'src/core/intcache.c',
  'src/core/regionalloc.c',
)

# Native call backend-specific source
if nativecall_backend == 'libffi'
  core_sources += files('src/core/nativecall_libffi.c')
else
  core_sources += files('src/core/nativecall_dyncall.c')
endif

# GC sources
gc_sources = files(
  'src/gc/orchestrate.c',
  'src/gc/allocation.c',
  'src/gc/worklist.c',
  'src/gc/roots.c',
  'src/gc/collect.c',
  'src/gc/gen2.c',
  'src/gc/wb.c',
  'src/gc/objectid.c',
  'src/gc/finalize.c',
  'src/gc/debug.c',
)

# I/O sources
io_sources = files(
  'src/io/io.c',
  'src/io/eventloop.c',
  'src/io/syncfile.c',
  'src/io/syncsocket.c',
  'src/io/fileops.c',
  'src/io/dirops.c',
  'src/io/procops.c',
  'src/io/timers.c',
  'src/io/filewatchers.c',
  'src/io/signals.c',
  'src/io/asyncsocket.c',
  'src/io/asyncsocketudp.c',
)

# 6model sources
model6_sources = files(
  'src/6model/reprs.c',
  'src/6model/reprconv.c',
  'src/6model/containers.c',
  'src/6model/parametric.c',
  'src/6model/6model.c',
  'src/6model/bootstrap.c',
  'src/6model/sc.c',
  'src/6model/serialization.c',
  # REPRs
  'src/6model/reprs/MVMString.c',
  'src/6model/reprs/VMArray.c',
  'src/6model/reprs/MVMHash.c',
  'src/6model/reprs/MVMCFunction.c',
  'src/6model/reprs/KnowHOWREPR.c',
  'src/6model/reprs/KnowHOWAttributeREPR.c',
  'src/6model/reprs/P6str.c',
  'src/6model/reprs/P6opaque.c',
  'src/6model/reprs/MVMCode.c',
  'src/6model/reprs/MVMOSHandle.c',
  'src/6model/reprs/MVMCompUnit.c',
  'src/6model/reprs/MVMStaticFrame.c',
  'src/6model/reprs/P6int.c',
  'src/6model/reprs/P6num.c',
  'src/6model/reprs/Uninstantiable.c',
  'src/6model/reprs/HashAttrStore.c',
  'src/6model/reprs/MVMThread.c',
  'src/6model/reprs/MVMIter.c',
  'src/6model/reprs/MVMContext.c',
  'src/6model/reprs/SCRef.c',
  'src/6model/reprs/P6bigint.c',
  'src/6model/reprs/NFA.c',
  'src/6model/reprs/MVMException.c',
  'src/6model/reprs/MVMDLLSym.c',
  'src/6model/reprs/MVMContinuation.c',
  'src/6model/reprs/NativeCall.c',
  'src/6model/reprs/CPointer.c',
  'src/6model/reprs/CStr.c',
  'src/6model/reprs/CArray.c',
  'src/6model/reprs/CStruct.c',
  'src/6model/reprs/CUnion.c',
  'src/6model/reprs/ReentrantMutex.c',
  'src/6model/reprs/ConditionVariable.c',
  'src/6model/reprs/Semaphore.c',
  'src/6model/reprs/ConcBlockingQueue.c',
  'src/6model/reprs/MVMAsyncTask.c',
  'src/6model/reprs/MVMNull.c',
  'src/6model/reprs/CPPStruct.c',
  'src/6model/reprs/NativeRef.c',
  'src/6model/reprs/MultiDimArray.c',
  'src/6model/reprs/Decoder.c',
  'src/6model/reprs/MVMSpeshLog.c',
  'src/6model/reprs/MVMStaticFrameSpesh.c',
  'src/6model/reprs/MVMSpeshCandidate.c',
  'src/6model/reprs/MVMCapture.c',
  'src/6model/reprs/MVMTracked.c',
  'src/6model/reprs/MVMStat.c',
)

# Dispatcher sources
disp_sources = files(
  'src/disp/boot.c',
  'src/disp/registry.c',
  'src/disp/inline_cache.c',
  'src/disp/program.c',
  'src/disp/syscall.c',
  'src/disp/resume.c',
)

# Specializer sources
spesh_sources = files(
  'src/spesh/dump.c',
  'src/spesh/graph.c',
  'src/spesh/codegen.c',
  'src/spesh/manipulate.c',
  'src/spesh/args.c',
  'src/spesh/usages.c',
  'src/spesh/facts.c',
  'src/spesh/optimize.c',
  'src/spesh/dead_bb_elimination.c',
  'src/spesh/dead_ins_elimination.c',
  'src/spesh/deopt.c',
  'src/spesh/log.c',
  'src/spesh/threshold.c',
  'src/spesh/inline.c',
  'src/spesh/osr.c',
  'src/spesh/lookup.c',
  'src/spesh/iterator.c',
  'src/spesh/worker.c',
  'src/spesh/debug.c',
  'src/spesh/stats.c',
  'src/spesh/plan.c',
  'src/spesh/arg_guard.c',
  'src/spesh/frame_walker.c',
  'src/spesh/pea.c',
  'src/spesh/disp.c',
)

# String processing sources
strings_sources = files(
  'src/strings/decode_stream.c',
  'src/strings/ascii.c',
  'src/strings/parse_num.c',
  'src/strings/utf8.c',
  'src/strings/utf8_c8.c',
  'src/strings/nfg.c',
  'src/strings/ops.c',
  'src/strings/unicode.c',
  'src/strings/normalize.c',
  'src/strings/latin1.c',
  'src/strings/utf16.c',
  'src/strings/windows1252.c',
  'src/strings/shiftjis.c',
  'src/strings/gb2312.c',
  'src/strings/gb18030.c',
)

# Math sources
math_sources = files('src/math/bigintops.c')

# Profiler sources
profiler_sources = files(
  'src/profiler/instrument.c',
  'src/profiler/log.c',
  'src/profiler/profile.c',
  'src/profiler/heapsnapshot.c',
  'src/profiler/telemeh.c',
  'src/profiler/configuration.c',
)

# Instrumentation sources
instrument_sources = files(
  'src/instrument/crossthreadwrite.c',
  'src/instrument/line_coverage.c',
)

# Platform-agnostic sources
platform_sources = files(
  'src/platform/sys.c',
  'src/platform/random.c',
  'src/platform/memmem32.c',
  'src/platform/malloc_trim.c',
)

# Debug sources
debug_sources = files('src/debug/debugserver.c')

# Main VM source
moar_sources = files('src/moar.c')

# Platform-specific sources
if is_windows
  platform_sources += files(
    'src/platform/win32/mmap.c',
    'src/platform/win32/time.c',
    'src/platform/win32/io.c',
    'src/platform/win32/fork.c',
  )
else
  platform_sources += files(
    'src/platform/posix/mmap.c',
    'src/platform/posix/time.c',
    'src/platform/posix/io.c',
    'src/platform/posix/fork.c',
  )
endif

# JIT sources
jit_sources = []
if jit_enabled
  jit_sources += files(
    'src/jit/graph.c',
    'src/jit/label.c',
    'src/jit/compile.c',
    'src/jit/dump.c',
    'src/jit/expr.c',
    'src/jit/tile.c',
    'src/jit/linear_scan.c',
    'src/jit/interface.c',
  )

  if host_cpu == 'x86_64'
    jit_sources += files(
      'src/jit/x64/arch.c',
      # Note: src/jit/x64/emit.c is generated from .dasc file
      # TODO: Add DynASM processing for emit.dasc
    )
  else
    jit_sources += files('src/jit/stub.c')
  endif
else
  jit_sources += files('src/jit/stub.c')
endif

# Combine all sources
libmoar_sources = [
  config_c,
  core_sources,
  gc_sources,
  io_sources,
  model6_sources,
  disp_sources,
  spesh_sources,
  strings_sources,
  math_sources,
  profiler_sources,
  instrument_sources,
  platform_sources,
  debug_sources,
  moar_sources,
  jit_sources,
]

# ============================================================================
# Library Target
# ============================================================================

# Build libmoar (shared or static)
libmoar = library('moar',
  libmoar_sources,
  include_directories: moar_inc,
  dependencies: moar_deps,
  install: true,
  version: meson.project_version(),
  soversion: version_major.to_string(),
  c_args: is_windows ? [] : ['-fPIC'],
  # For generated files
  sources: jit_enabled ? jit_gen_files : [],
)

# Create a dependency object for libmoar
libmoar_dep = declare_dependency(
  link_with: libmoar,
  include_directories: moar_inc,
  dependencies: moar_deps,
)

# ============================================================================
# Executable Target
# ============================================================================

# Build moar executable
moar_exe = executable('moar',
  files('src/main.c'),
  include_directories: moar_inc,
  dependencies: libmoar_dep,
  install: true,
  export_dynamic: not is_windows,  # Allow dynamic symbol resolution
)

# ============================================================================
# Pkg-config File
# ============================================================================

pkg.generate(libmoar,
  name: 'moar',
  description: 'MoarVM - A VM for NQP and Rakudo Perl 6',
  url: 'https://moarvm.org',
  subdirs: 'moar',
  extra_cflags: ['-DMVM_BUILD'],
)

# ============================================================================
# Installation Rules
# ============================================================================

# Install main header
install_headers('src/moar.h', subdir: 'moar')

# Install generated config header
install_headers(config_h, subdir: 'moar')

# Install MAST NQP libraries (if they exist)
# These are needed for tools that compile to MoarVM bytecode
if fs.is_dir('lib/MAST')
  install_subdir('lib/MAST',
    install_dir: mastdir,
    strip_directory: true,
  )
endif

# ============================================================================
# Summary
# ============================================================================

message('='*60)
message('MoarVM Meson Build Configuration Summary')
message('='*60)
message('Version:               ' + meson.project_version())
message('Prefix:                ' + get_option('prefix'))
message('Libdir:                ' + get_option('prefix') / libdir)
message('Bindir:                ' + get_option('prefix') / bindir)
message('MAST dir:              ' + get_option('prefix') / mastdir)
message('')
message('Compiler:              ' + cc.get_id() + ' ' + cc.version())
message('Build type:            ' + get_option('buildtype'))
message('Optimization:          ' + get_option('optimization'))
message('Debug:                 ' + get_option('debug').to_string())
message('')
message('JIT:                   ' + (jit_enabled ? 'enabled' : 'disabled'))
if jit_enabled
  message('  Architecture:        ' + host_cpu)
endif
message('Static library:        ' + opt_static.to_string())
message('Relocatable:           ' + opt_relocatable.to_string())
message('')
message('Memory allocator:      ' + (use_mimalloc ? 'mimalloc' : 'libc malloc'))
message('C11 atomics:           ' + (use_c11_atomics ? 'yes' : 'libatomic_ops'))
message('Nativecall backend:    ' + nativecall_backend)
message('')
message('Sanitizers:')
message('  ASAN:                ' + opt_asan.to_string())
message('  UBSAN:               ' + opt_ubsan.to_string())
message('  TSAN:                ' + opt_tsan.to_string())
message('Coverage:              ' + opt_coverage.to_string())
message('='*60)
message('')
message('Build with: meson compile -C builddir')
message('Install with: meson install -C builddir')
message('')
