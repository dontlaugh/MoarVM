
subdir('gen')
# TODO(cm): src/meson.build:6:0: ERROR: Non-existent build file 'src/6model/meson.build'
#    work through each of these, add a meson.build
#    to each, exporting sources or headers if need be
subdir('6model')
subdir('core')
subdir('debug')
subdir('disp')
subdir('gc')
subdir('instrument')
subdir('io')
subdir('jit')
subdir('math')
subdir('platform')
subdir('profiler')
subdir('spesh')
subdir('strings')


srcs = ['main.c', 'moar.c']

moar_deps = [
    dyncall_s_lib,
    mimalloc_lib,
    libuv_lib_a,
    ryu_lib,
    libtommath_lib,
]

executable('moar',
           sources : srcs,
           dependencies : moar_deps,
           include_directories: [dynload_inc_dir])
