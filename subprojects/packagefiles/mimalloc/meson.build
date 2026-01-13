project('mimalloc', 'c', version: '2.1')

mimalloc_sources = files(
  'src/alloc.c',
  'src/alloc-aligned.c',
  'src/alloc-posix.c',
  'src/arena.c',
  'src/bitmap.c',
  'src/heap.c',
  'src/init.c',
  'src/libc.c',
  'src/options.c',
  'src/os.c',
  'src/page.c',
  'src/random.c',
  'src/segment.c',
  'src/segment-map.c',
  'src/stats.c',
  'src/prim/prim.c',
)

mimalloc_lib = static_library('mimalloc',
  mimalloc_sources,
  include_directories: include_directories('include'),
  c_args: ['-DMI_STATIC_LIB'],
)

mimalloc_dep = declare_dependency(
  link_with: mimalloc_lib,
  include_directories: include_directories('include'),
)

meson.override_dependency('mimalloc', mimalloc_dep)
