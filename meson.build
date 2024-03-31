project('MoarVM', 'c')

# subdir('src')

cmake = import('cmake')

dyncall_subproject = cmake.subproject('dyncall')
dyncall_s_lib = dyncall_subproject.dependency('dyncall_s')
shared_library('dyncall_s', dependencies : [dyncall_s_lib])
shared_library('dyncallback_s', dependencies : [dyncall_s_lib])
shared_library('dynload_s', dependencies : [dyncall_s_lib])

mimalloc_subproject = cmake.subproject('mimalloc')
mimalloc_lib = mimalloc_subproject.dependency('mimalloc')
shared_library('mimalloc', dependencies : [mimalloc_lib])
static_library('mimalloc-static', dependencies : [mimalloc_lib])

# Note: we could use WrapDB package management for libuv;
libuv_opts = cmake.subproject_options()
# libuv_opts.add_cmake_defines({'LIBUV_BUILD_SHARED': true})
libuv_subproject = cmake.subproject('libuv', options : libuv_opts)
message('CMake targets:\n - ' + '\n - '.join(libuv_subproject.target_list()))
# if set LIBUV_BUILD_SHARED we can build a shared lib
# libuv_lib = mimalloc_subproject.dependency('uv')
# shared_library('libuv', dependencies : [libuv_lib])
libuv_lib_a = libuv_subproject.dependency('uv_a')
static_library('uv_a', dependencies : [libuv_lib_a])