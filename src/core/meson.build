

core_srcs = files(
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
#  'nativecall_libffi.c',   # TODO: add behind config option; an alternative to dyncall
  'ops.c',
  'ptr_hash_table.c',
  'regionalloc.c',
  'str_hash_table.c',
  'threadcontext.c',
  'threads.c',
  'uni_hash_table.c',
  'validation.c',
)

core_inc = [include_directories('.', '..'), mimalloc_inc, dynload_inc_dir ]

core_deps_extra = [libuv_dep, dynload_s_lib, libtommath_lib, ryu_lib]

core_lib = static_library('core_a', core_srcs, dependencies : core_deps_extra,
                          include_directories : core_inc)

core_dep = declare_dependency(link_with : core_lib,
                             include_directories : core_inc)