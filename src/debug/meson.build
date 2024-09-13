
debug_srcs = [
  'debugserver.c',
]

debug_lib = static_library('debug', debug_srcs)

debug_dep = declare_dependency(link_with : debug_lib,
  include_directories : include_directories('.'))
