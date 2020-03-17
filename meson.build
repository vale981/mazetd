project('mazetd', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3',
                     'cpp_std=c++17'])


project_sources = []
project_header_files = []

inc = [include_directories('subprojects/entt/single_include/'),
      include_directories('src')]

subdir('src')

sfml_deps = []
sfml_modules = ['window', 'graphics', 'system']

foreach mod : sfml_modules
  sfml_deps += [dependency('sfml-' + mod)]
endforeach

spdlog_proj = subproject('spdlog')
spdlog_dep = spdlog_proj.get_variable('spdlog_dep')

# This adds the clang format file to the build directory
configure_file(input : '.clang-format',
               output : '.clang-format',
               copy: true)

exe = executable('mazetd', project_sources,
  include_directories: inc,
  dependencies : sfml_deps + [spdlog_dep])

run_target('format',
  command : ['clang-format','-i','-style=file', project_sources, project_header_files])

test('basic', exe)

regex = '^((?!(third_party|tests|benchmarks|gtest|subprojects)).)*$'

# This adds clang tidy support
configure_file(input : '.clang-tidy',
               output : '.clang-tidy',
               copy : true)

run_target('tidy',
  command : ['run-clang-tidy.py','-fix', '-j' , '8', 'files', regex ,'-format', '-p='+ meson.build_root()])
