
instrument_srcs = [
  'crossthreadwrite.c',
  'line_coverage.c',
]

instrument_lib = static_library('profiler', instrument_srcs)

instrument_dep = declare_dependency(link_with : instrument_lib,
                                    include_directories : include_directories('.'))
