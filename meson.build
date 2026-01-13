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

# Feature detection will be added in Phase 1.2
# This is a placeholder for the basic structure

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

# Subsequent phases will add:
# - Feature detection (Phase 1.2)
# - Dependency setup (Phase 2)
# - Code generation (Phase 3)
# - Source compilation (Phase 4)
# - Installation rules (Phase 6)
