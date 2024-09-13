
# Output cannot contain a path segment, but input can?

configure_file(input : '../../build/config.h.in',
               output : 'config.h',
               configuration : conf_data)
configure_file(input : '../../build/config.c.in',
               output : 'config.c',
               configuration : conf_data)

# these don't exist on a clean checkout
#gen_configh_lib = static_library('gen', 'config.c', include_directories : ['../..', '../../..'])  # generated source file
#
#gen_configh_dep = declare_dependency(link_with : gen_configh_lib,
#                               include_directories : ['../..', '../../..'])
