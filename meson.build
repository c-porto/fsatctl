project(
  'fsatctl',
  'cpp',
  version: '0.2.1',
  default_options: [
    'warning_level=3',
    'cpp_std=c++20',
    'optimization=3',
    'buildtype=debug',
  ],
)

fsatctl_depencies = []

add_project_arguments(
  [
    '-Wno-unused-parameter',
    '-Wno-unused-value',
    '-Wno-missing-field-initializers',
    '-Wno-narrowing',
    '-Wno-pointer-arith',
  ],
  language: 'cpp',
)

sources = files(
  'src/main.cpp',
  'src/service.cpp',
  'src/arg_parser.cpp',
)

inc_dir = include_directories('include')

json = dependency('nlohmann_json')

fsatctl_depencies += json

bin = executable(
  'fsatctl',
  sources: sources,
  include_directories: inc_dir,
  dependencies: fsatctl_depencies,
  install: true
)

inst_srcs = get_option('install_src')
inst_srcs_dir = get_option('src_install_dir')

if inst_srcs
  install_subdir('include', install_dir: inst_srcs_dir)
  install_subdir('src', install_dir: inst_srcs_dir)
endif
