project('dyncall', 'c', version: '1.4')

host_cpu = host_machine.cpu_family()
host_system = host_machine.system()

# Common dyncall source files
dyncall_sources = files(
  'dyncall/dyncall_api.c',
  'dyncall/dyncall_callf.c',
  'dyncall/dyncall_callvm.c',
  'dyncall/dyncall_callvm_base.c',
  'dyncall/dyncall_struct.c',
  'dyncall/dyncall_vector.c',
)

# Architecture-specific dyncall sources
if host_cpu == 'x86_64'
  dyncall_sources += files(
    'dyncall/dyncall_callvm_x64.c',
    'dyncall/dyncall_call_x64.S',
  )
elif host_cpu == 'x86'
  dyncall_sources += files(
    'dyncall/dyncall_callvm_x86.c',
    'dyncall/dyncall_call_x86.S',
  )
elif host_cpu == 'aarch64'
  dyncall_sources += files(
    'dyncall/dyncall_callvm_arm64.c',
    'dyncall/dyncall_call_arm64.S',
  )
elif host_cpu == 'arm'
  dyncall_sources += files(
    'dyncall/dyncall_callvm_arm32_arm_armhf.c',
    'dyncall/dyncall_call_arm32_arm_armhf.S',
  )
endif

# Dynload sources (dynamic loading)
if host_system == 'windows'
  dynload_sources = files(
    'dynload/dynload.c',
    'dynload/dynload_syms.c',
    'dynload/dynload_syms_pe.c',
    'dynload/dynload_windows.c',
  )
elif host_system == 'darwin'
  dynload_sources = files(
    'dynload/dynload.c',
    'dynload/dynload_syms.c',
    'dynload/dynload_syms_mach-o.c',
    'dynload/dynload_unix.c',
  )
else  # Linux and other Unix-likes
  dynload_sources = files(
    'dynload/dynload.c',
    'dynload/dynload_syms.c',
    'dynload/dynload_syms_elf.c',
    'dynload/dynload_unix.c',
  )
endif

# Dyncallback sources (callback support)
dyncallback_sources = files(
  'dyncallback/dyncall_callback.c',
  'dyncallback/dyncall_args.c',
  'dyncallback/dyncall_thunk.c',
)

# Memory allocation for callbacks
if host_system == 'windows'
  dyncallback_sources += files(
    'dyncallback/dyncall_alloc_wx.c',
    'dyncallback/dyncall_alloc_wx_win32.c',
  )
else
  dyncallback_sources += files(
    'dyncallback/dyncall_alloc_wx.c',
    'dyncallback/dyncall_alloc_wx_mmap.c',
  )
endif

# Architecture-specific callback sources
if host_cpu == 'x86_64'
  dyncallback_sources += files(
    'dyncallback/dyncall_callback_x64.c',
    'dyncallback/dyncall_callback_x64.S',
    'dyncallback/dyncall_args_x64.c',
    'dyncallback/dyncall_thunk_x64.c',
  )
elif host_cpu == 'x86'
  dyncallback_sources += files(
    'dyncallback/dyncall_callback_x86.c',
    'dyncallback/dyncall_callback_x86.S',
    'dyncallback/dyncall_args_x86.c',
    'dyncallback/dyncall_thunk_x86.c',
  )
elif host_cpu == 'aarch64'
  dyncallback_sources += files(
    'dyncallback/dyncall_callback_arm64.c',
    'dyncallback/dyncall_callback_arm64.S',
    'dyncallback/dyncall_args_arm64.c',
    'dyncallback/dyncall_thunk_arm64.c',
  )
elif host_cpu == 'arm'
  dyncallback_sources += files(
    'dyncallback/dyncall_callback_arm32.c',
    'dyncallback/dyncall_callback_arm32_arm_gas.S',
    'dyncallback/dyncall_args_arm32.c',
    'dyncallback/dyncall_thunk_arm32.c',
  )
endif

dyncall_inc = include_directories('dyncall', 'dynload', 'dyncallback')

# Build all three libraries
dyncall_lib = static_library('dyncall_s',
  dyncall_sources,
  include_directories: dyncall_inc,
)

dynload_lib = static_library('dynload_s',
  dynload_sources,
  include_directories: dyncall_inc,
)

dyncallback_lib = static_library('dyncallback_s',
  dyncallback_sources,
  include_directories: dyncall_inc,
)

# Combined dependency - link all three libraries
dyncall_dep = declare_dependency(
  link_with: [dyncall_lib, dynload_lib, dyncallback_lib],
  include_directories: dyncall_inc,
)

meson.override_dependency('dyncall', dyncall_dep)
