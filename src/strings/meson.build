
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

strings_lib = static_library('strings', strings_srcs)

strings_dep = declare_dependency(link_with : strings_lib,
                              include_directories : include_directories('.'))
