
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

gc_inc =  [include_directories('.', '..'), mimalloc_inc, dynload_inc_dir ]

gc_deps_extra = [libuv_dep, dynload_s_lib, libtommath_lib, ryu_lib]

gc_lib = static_library('gc_a', gc_srcs,  dependencies : gc_deps_extra,
                        include_directories : gc_inc)

gc_dep = declare_dependency(link_with : gc_lib, include_directories : gc_inc)