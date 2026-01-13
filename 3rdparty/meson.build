# 3rd party vendored libraries
# These are small libraries included directly as source in the repository

# SHA-1 implementation
sha1_sources = files('sha1/sha1.c')
sha1_inc = include_directories('sha1')

sha1_lib = static_library('sha1',
  sha1_sources,
  include_directories: sha1_inc,
  install: false,
)

sha1_dep = declare_dependency(
  link_with: sha1_lib,
  include_directories: sha1_inc,
)

# FreeBSD memmem implementation (for platforms that don't have it)
freebsd_sources = []
freebsd_inc = include_directories('freebsd')

# Check if memmem is available in the system
has_memmem = cc.has_function('memmem',
  prefix: '#define _GNU_SOURCE\n#include <string.h>')

if not has_memmem
  freebsd_sources += files('freebsd/memmem.c')
endif

if freebsd_sources.length() > 0
  freebsd_lib = static_library('freebsd_compat',
    freebsd_sources,
    include_directories: freebsd_inc,
    install: false,
  )

  freebsd_dep = declare_dependency(
    link_with: freebsd_lib,
    include_directories: freebsd_inc,
  )
else
  freebsd_dep = declare_dependency(
    include_directories: freebsd_inc,
  )
endif

# MS integer types (for old MSVC)
# This is header-only, so just provide the include directory
msinttypes_inc = include_directories('msinttypes')
msinttypes_dep = declare_dependency(
  include_directories: msinttypes_inc,
)
