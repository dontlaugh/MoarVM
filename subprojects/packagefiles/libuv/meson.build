project('libuv', 'c', version: '1.x')

cc = meson.get_compiler('c')
host_system = host_machine.system()

# Required defines for POSIX types (pthread_rwlock_t, SSIZE_MAX, lstat, etc.)
uv_c_args = ['-D_GNU_SOURCE']

# Thread library
thread_dep = dependency('threads')

uv_sources = files(
  'src/fs-poll.c',
  'src/idna.c',
  'src/inet.c',
  'src/random.c',
  'src/strscpy.c',
  'src/strtok.c',
  'src/thread-common.c',
  'src/threadpool.c',
  'src/timer.c',
  'src/uv-common.c',
  'src/uv-data-getter-setters.c',
  'src/version.c',
)

if host_system == 'linux'
  uv_sources += files(
    'src/unix/async.c',
    'src/unix/core.c',
    'src/unix/dl.c',
    'src/unix/fs.c',
    'src/unix/getaddrinfo.c',
    'src/unix/getnameinfo.c',
    'src/unix/linux.c',
    'src/unix/loop.c',
    'src/unix/loop-watcher.c',
    'src/unix/pipe.c',
    'src/unix/poll.c',
    'src/unix/process.c',
    'src/unix/procfs-exepath.c',
    'src/unix/proctitle.c',
    'src/unix/random-devurandom.c',
    'src/unix/random-getrandom.c',
    'src/unix/random-sysctl-linux.c',
    'src/unix/signal.c',
    'src/unix/stream.c',
    'src/unix/tcp.c',
    'src/unix/thread.c',
    'src/unix/tty.c',
    'src/unix/udp.c',
  )
endif

uv_inc = include_directories('include', 'src')

libuv_lib = static_library('uv',
  uv_sources,
  include_directories: uv_inc,
  c_args: uv_c_args,
  dependencies: [thread_dep],
)

libuv_dep = declare_dependency(
  link_with: libuv_lib,
  include_directories: include_directories('include'),
  dependencies: [thread_dep],
)

meson.override_dependency('libuv', libuv_dep)
