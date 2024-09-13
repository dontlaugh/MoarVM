
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

io_lib = static_library('io', io_srcs)

io_dep = declare_dependency(link_with : io_lib,
                            include_directories : include_directories('.'))
