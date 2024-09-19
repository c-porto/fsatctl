project('fsatctl', 'cpp',
  version : '0.1',
  default_options : [
    'warning_level=3',
    'cpp_std=c++17',
    'optimization=3',
    'buildtype=debug',
  ])

  add_project_arguments(
  [
    '-Wno-unused-parameter',
    '-Wno-unused-value',
    '-Wno-missing-field-initializers',
    '-Wno-narrowing',
    '-Wno-pointer-arith',
  ],
  language: 'cpp')

sources = files(
    'src/main.cpp',
    'src/service.cpp',
  )

inc_dir = include_directories('include')

exe = executable('fsatctl', sources, include_directories: inc_dir)
