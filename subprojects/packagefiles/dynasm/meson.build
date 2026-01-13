project('dynasm', 'c', version: '1.0')

cc = meson.get_compiler('c')

# Build minilua - the tiny Lua interpreter used to run dynasm
minilua = executable('minilua',
  'minilua.c',
  c_args: ['-O2'],
  link_args: cc.get_supported_link_arguments(['-lm']),
  native: true,  # Build for host machine (for use during build)
)

# Export information about dynasm for use in parent project
dynasm_dep = declare_dependency(
  include_directories: include_directories('.'),
)

meson.override_dependency('dynasm', dynasm_dep)

# Make minilua available to parent
# The parent can use subproject('dynasm').get_variable('minilua')
