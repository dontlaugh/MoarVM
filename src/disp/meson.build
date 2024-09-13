
disp_srcs = [
  'boot.c',
  'inline_cache.c',
  'program.c',
  'registry.c',
  'resume.c',
  'syscall.c',
]


disp_lib = static_library('sixmodel', disp_srcs)

disp_dep = declare_dependency(link_with : disp_lib,
                              include_directories : include_directories('.'))

