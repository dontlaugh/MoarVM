project('libatomic_ops', 'c', version: '7.8')

# libatomic_ops - mostly header-only with some optional compiled components
libatomic_ops_dep = declare_dependency(
  include_directories: include_directories('src'),
)

meson.override_dependency('libatomic_ops', libatomic_ops_dep)
