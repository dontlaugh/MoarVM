project('ryu', 'c', version: '1.0')

# ryu is a float-to-string conversion library
ryu_sources = files(
  'ryu/d2s.c',
  'ryu/f2s.c',
)

ryu_lib = static_library('ryu',
  ryu_sources,
  include_directories: include_directories('.'),
)

ryu_dep = declare_dependency(
  link_with: ryu_lib,
  include_directories: include_directories('.'),
)

meson.override_dependency('ryu', ryu_dep)
