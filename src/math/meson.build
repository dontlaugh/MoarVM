
math_srcs = [
  'bigintops.c',
]


math_inc = [include_directories('.', '..'), mimalloc_inc, dynload_inc_dir ]

math_deps_extra = [libuv_dep, dynload_s_lib, libtommath_lib, ryu_lib]

math_lib = static_library('math_a', math_srcs, dependencies : math_deps_extra,
                          include_directories : math_inc)

math_dep = declare_dependency(link_with : math_lib,
                              include_directories : math_inc)

