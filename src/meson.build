

#subdir('gen')
moar_src_inc = include_directories('.')

subdir('6model')
subdir('core')
#subdir('debug')  # needed?
subdir('disp')
subdir('gc')
subdir('instrument')
subdir('io')
subdir('jit')
subdir('math')
subdir('platform')
#subdir('profiler')
subdir('spesh')
subdir('strings')


main_srcs = ['main.c', 'moar.c', 'config.c']  # todo: make config.c actually generated

moar_thirdparty_deps = [
    dyncall_s_lib,
    dynload_s_lib,
    mimalloc_dep,
    libuv_dep,
    ryu_lib,
    libtommath_lib,
]

moar_deps = [
    core_dep,
#    debug_dep,  # do we need this?
    #gen_configh_dep,    # doesn't exist on a clean checkout
    gc_dep,
    io_dep,
    sixmodel_dep,
    sixmodel_reprs_dep,
    disp_dep,
    spesh_dep,
    strings_dep,
    math_dep,
#    profiler_dep,
    instrument_dep,
    platform_dep,
]

moar_inc_dirs = [
    dynload_inc_dir,
    include_directories('.'),
    mimalloc_inc,
]

executable('moarvm',
           sources : main_srcs,
           dependencies : moar_thirdparty_deps + moar_deps,
           include_directories: moar_inc_dirs)
