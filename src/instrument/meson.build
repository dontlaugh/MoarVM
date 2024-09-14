
instrument_srcs = [
  'crossthreadwrite.c',
  'line_coverage.c',
]


instrument_inc = [include_directories('.', '..'), mimalloc_inc, dynload_inc_dir ]

instrument_deps_extra = [libuv_dep, dynload_s_lib, libtommath_lib, ryu_lib]

instrument_lib = static_library('instrument_a', instrument_srcs,
                                dependencies : instrument_deps_extra,
                                include_directories : instrument_inc)

instrument_dep = declare_dependency(link_with : instrument_lib,
                                    include_directories : instrument_inc)
