

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

sixmodel_lib = static_library('sixmodel', sixmodel_srcs)

sixmodel_dep = declare_dependency(link_with : sixmodel_lib,
                            include_directories : include_directories('.'))

