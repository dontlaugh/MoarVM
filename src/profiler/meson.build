
profiler_srcs = [
  'configuration.c',
  'heapsnapshot.c',
  'instrument.c',
  'log.c',
  'profile.c',
  'telemeh.c',
]

profiler_lib = static_library('profiler', profiler_srcs)

profiler_dep = declare_dependency(link_with : profiler_lib,
                              include_directories : include_directories('.'))
