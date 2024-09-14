

subdir('reprs')

sixmodel_srcs = [
    '6model.c',
    'bootstrap.c',
    'containers.c',
    'parametric.c',
    'reprconv.c',
    'reprs.c',
    'sc.c',
    'serialization.c',
]

sixmodel_inc_dirs_extra = [include_directories('.', '..'),
                           mimalloc_inc,
                           dynload_inc_dir,
                           ]

sixmodel_deps_extra = [
    libuv_dep,
    dynload_s_lib,
    libtommath_lib,
    sha1_dep,
]

sixmodel_lib = static_library('sixmodel_a', sixmodel_srcs,
                              dependencies : sixmodel_deps_extra,
                              include_directories : sixmodel_inc_dirs_extra)

sixmodel_dep = declare_dependency(link_with : sixmodel_lib,
                            include_directories : sixmodel_inc_dirs_extra)

