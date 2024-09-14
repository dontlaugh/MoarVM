
subdir('siphash')

strings_srcs = [
  'ascii.c',
  'decode_stream.c',
  'gb18030.c',
  'gb2312.c',
  'latin1.c',
  'nfg.c',
  'normalize.c',
  'ops.c',
  'parse_num.c',
  'shiftjis.c',
  'unicode.c',
  'unicode_db.c',
  'unicode_ops.c',
  'unicode_uca.c',
  'utf16.c',
  'utf8.c',
  'utf8_c8.c',
  'windows1252.c',
]

strings_inc = [include_directories('.', '..'), mimalloc_inc, dynload_inc_dir , moar_src_inc,]

strings_deps_extra = [ libuv_dep, dynload_s_lib, libtommath_lib, ryu_lib]

strings_lib = static_library('strings_a', strings_srcs, dependencies : strings_deps_extra,
                          include_directories : strings_inc)

strings_dep = declare_dependency(link_with : strings_lib,
                              include_directories : strings_inc)
