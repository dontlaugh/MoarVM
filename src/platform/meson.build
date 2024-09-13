
# pick a platform implementation
if host_machine.system() == 'windows'
    subdir('win32')
    platform_impl_dep = platform_win32_dep
else
    subdir('posix')
    platform_impl_dep = platform_posix_dep
endif

platform_srcs = [
  'malloc_trim.c',
  'memmem32.c',
  'random.c',
  'sys.c',
]

platform_lib = static_library('platform', platform_srcs)

platform_dep = declare_dependency(link_with : platform_lib,
                                        dependencies : [platform_impl_dep],
                                        include_directories : include_directories('.'))