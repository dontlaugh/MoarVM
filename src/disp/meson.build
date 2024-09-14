
disp_srcs = [
  'boot.c',
  'inline_cache.c',
  'program.c',
  'registry.c',
  'resume.c',
  'syscall.c',
]

disp_inc = [include_directories('.'), dynload_inc_dir, mimalloc_inc, moar_src_inc]
disp_deps_extra = [libuv_dep, dynload_s_lib, libtommath_lib, ryu_lib]
disp_lib = static_library('sixmodel', disp_srcs,
                          dependencies : disp_deps_extra,
                          include_directories : disp_inc)

disp_dep = declare_dependency(link_with : disp_lib,
                              include_directories : disp_inc)

