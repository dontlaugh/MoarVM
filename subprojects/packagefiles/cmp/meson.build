project('cmp', 'c', version: '20')

# cmp is a MessagePack library - single source file
cmp_lib = static_library('cmp',
  files('cmp.c'),
  include_directories: include_directories('.'),
)

cmp_dep = declare_dependency(
  link_with: cmp_lib,
  include_directories: include_directories('.'),
)

meson.override_dependency('cmp', cmp_dep)
