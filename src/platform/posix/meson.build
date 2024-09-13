
srcs = files(
  'fork.c',
  'io.c',
  'mmap.c',
  'time.c',
)

lib = static_library('posix', srcs)

platform_posix_dep = declare_dependency(link_with : lib,
                                        include_directories : include_directories('.'))