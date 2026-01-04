project('coin', 'c',
  version: '0.0.1',
  default_options: ['warning_level=3']
)

srcs = files()

sdl_dep = dependency('SDL3')
sdl_ttf_dep = dependency('SDL3_ttf')

cc = meson.get_compiler('c')

subdir('src')

include = include_directories(
  'include'
)

executable('coin', srcs,
  include_directories: include,
  dependencies: [
    sdl_dep,
    sdl_ttf_dep,
    cc.find_library('m')
  ]
)
