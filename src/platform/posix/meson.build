
posix_srcs = files(
  'fork.c',
  'io.c',
  'mmap.c',
  'time.c',
)

posix_inc = [include_directories('.', '../..'), mimalloc_inc, dynload_inc_dir ]

posix_deps_extra = [libuv_dep, dynload_s_lib, libtommath_lib, ryu_lib]

posix_lib = static_library('posix_a', posix_srcs, dependencies : posix_deps_extra,
                          include_directories : posix_inc)

posix_dep = declare_dependency(link_with : posix_lib,
                              include_directories : posix_inc)