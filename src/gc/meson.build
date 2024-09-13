
gc_srcs = [
  'allocation.c',
  'collect.c',
  'debug.c',
  'finalize.c',
  'gen2.c',
  'objectid.c',
  'orchestrate.c',
  'roots.c',
  'wb.c',
  'worklist.c',
]

gc_lib = static_library('debug', gc_srcs)

gc_dep = declare_dependency(link_with : gc_lib,
                               include_directories : include_directories('.'))
