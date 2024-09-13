

srcs = files(
  'args.c',
  'bytecode.c',
  'bytecodedump.c',
  'callsite.c',
  'callstack.c',
  'coerce.c',
  'compunit.c',
  'continuation.c',
  'dll.c',
  'exceptions.c',
  'ext.c',
  'fixkey_hash_table.c',
  'frame.c',
  'hll.c',
  'index_hash_table.c',
  'intcache.c',
  'interp.c',
  'loadbytecode.c',
  'nativecall.c',
  'nativecall_dyncall.c',
  'nativecall_libffi.c',
  'ops.c',
  'ptr_hash_table.c',
  'regionalloc.c',
  'str_hash_table.c',
  'threadcontext.c',
  'threads.c',
  'uni_hash_table.c',
  'validation.c',
)

core_lib = static_library('core', srcs)

core_dep = declare_dependency(link_with : core_lib,
                             include_directories : include_directories('.'))