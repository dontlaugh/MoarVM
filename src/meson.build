
# todo: rename this to moar_h
subdir('gen')
config_h_inc = include_directories('.')

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


main_srcs = ['main.c', 'moar.c']

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
    debug_dep,
#    gen_configh_dep,    # doesn't exist on a clean checkout
    gc_dep,
    io_dep,
    sixmodel_dep,  # includes repr? need .h from here?
    sixmodel_reprs_dep,
    disp_dep,
    spesh_dep,
    strings_dep,
    math_dep,
    profiler_dep,
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
