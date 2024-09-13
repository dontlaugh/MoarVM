
math_srcs = [
'bigintops.c',
]


math_lib = static_library('sixmodel', math_srcs)

math_dep = declare_dependency(link_with : math_lib,
                                        include_directories : include_directories('.'))

