project('MoarVM', 'c')

# subdir('src')

cmake = import('cmake')

dyncall_subproject = cmake.subproject('dyncall')
dyncall_s_lib = dyncall_subproject.dependency('dyncall_s')
shared_library('dyncall_s', dependencies : [dyncall_s_lib])
shared_library('dyncallback_s', dependencies : [dyncall_s_lib])
shared_library('dynload_s', dependencies : [dyncall_s_lib])

