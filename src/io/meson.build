
io_srcs = [
  'asyncsocket.c',
  'asyncsocketudp.c',
  'dirops.c',
  'eventloop.c',
  'fileops.c',
  'filewatchers.c',
  'io.c',
  'procops.c',
  'signals.c',
  'syncfile.c',
  'syncsocket.c',
  'timers.c',
]
io_inc = [include_directories('.', '..'), mimalloc_inc, dynload_inc_dir ]
io_deps_extra = [libuv_dep, dynload_s_lib, libtommath_lib, ryu_lib]

io_lib = static_library('io_a', io_srcs, dependencies : io_deps_extra,
                        include_directories : io_inc)

io_dep = declare_dependency(link_with : io_lib,
                            include_directories : include_directories('.'))
