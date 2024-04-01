
subdir('gen')

srcs = ['main.c', 'moar.c']

moar_deps = [dyncall_s_lib, mimalloc_lib, libuv_lib_a, ryu_lib]

executable('moar', sources : srcs, dependencies : moar_deps)
