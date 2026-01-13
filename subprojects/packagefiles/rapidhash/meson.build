project('rapidhash', 'c', version: '1.0')

# rapidhash is a header-only library
# Include parent directory so that #include "rapidhash/rapidhash.h" works
rapidhash_dep = declare_dependency(
  include_directories: include_directories('..'),
)

meson.override_dependency('rapidhash', rapidhash_dep)
