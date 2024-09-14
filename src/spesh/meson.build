
spesh_srcs = [
  'arg_guard.c',
  'args.c',
  'codegen.c',
  'dead_bb_elimination.c',
  'dead_ins_elimination.c',
  'debug.c',
  'deopt.c',
  'disp.c',
  'dump.c',
  'facts.c',
  'frame_walker.c',
  'graph.c',
  'inline.c',
  'iterator.c',
  'log.c',
  'lookup.c',
  'manipulate.c',
  'optimize.c',
  'osr.c',
  'pea.c',
  'plan.c',
  'stats.c',
  'threshold.c',
  'usages.c',
  'worker.c',
]

spesh_inc = [include_directories('.', '..'), mimalloc_inc, dynload_inc_dir ]

spesh_deps_extra = [libuv_dep, dynload_s_lib, libtommath_lib, ryu_lib]

spesh_lib = static_library('spesh_a', spesh_srcs, dependencies : spesh_deps_extra,
                          include_directories : spesh_inc)

spesh_dep = declare_dependency(link_with : spesh_lib,
                              include_directories : spesh_inc)
