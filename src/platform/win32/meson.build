
srcs = files(
  'fork.c',
  'io.c',
  'mmap.c',
  'time.c',
)

lib = static_library('win32', srcs)

platform_win32_dep = declare_dependency(link_with : lib,
                                        include_directories : include_directories('.'))