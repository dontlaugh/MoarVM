
# pick a platform implementation
if host_machine.system() == 'windows'
    subdir('win32')
    platform_impl_dep = platform_win32_dep
else
    subdir('posix')
    platform_impl_dep = posix_dep
endif

platform_srcs = [
  'malloc_trim.c',
  'memmem32.c',
  'random.c',
  'sys.c',
]

platform_inc = [include_directories('.', '..'), mimalloc_inc, dynload_inc_dir, moar_src_inc]
platform_deps_extra = [platform_impl_dep, libuv_dep, dynload_s_lib, libtommath_lib, ryu_lib ]

platform_lib = static_library('platform', platform_srcs, dependencies : platform_deps_extra,
                               include_directories : platform_inc)

platform_dep = declare_dependency(link_with : platform_lib,
                                        dependencies : platform_deps_extra,
                                        include_directories : platform_inc)