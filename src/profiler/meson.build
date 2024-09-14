
profiler_srcs = [
  'configuration.c',
  'heapsnapshot.c',
  'instrument.c',
  'log.c',
  'profile.c',
  'telemeh.c',
]

profiler_inc = [include_directories('.', '..'), mimalloc_inc, dynload_inc_dir ]

profiler_deps_extra = [libuv_dep, dynload_s_lib, libtommath_lib, ryu_lib]

profiler_lib = static_library('profiler_a', profiler_srcs, dependencies : profiler_deps_extra,
                          include_directories : profiler_inc)

profiler_dep = declare_dependency(link_with : profiler_lib,
                              include_directories : profiler_inc)
