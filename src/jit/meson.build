
jit_srcs = [
  'compile.c',
  'dump.c',
  'expr.c',
  'graph.c',
  'interface.c',
  'label.c',
  'linear_scan.c',
  'stub.c',
  'tile.c',
]

subdir('x64')

jit_inc = include_directories('.')

jit_lib = static_library('jit_a', jit_srcs)

jit_stub_lib = static_library('jit_stub', ['stub.c'])

# todo: add dep from x64 subdir
jit_dep = declare_dependency(link_with : jit_lib,
                             dependencies : [],
                             include_directories : include_directories('.'))