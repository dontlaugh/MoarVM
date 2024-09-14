project('sha1', 'c')

sha1_inc = include_directories('.')
sha1_lib = static_library('sha1_a', sources : ['sha1.c'], include_directories: sha1_inc)

sha1_dep = declare_dependency(link_with : sha1_lib, include_directories : sha1_inc)