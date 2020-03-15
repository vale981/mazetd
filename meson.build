project('mazetd', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3',
                     'cpp_std=c++17'])

sfml_deps = []
sfml_modules = ['window', 'graphics', 'system']

foreach mod : sfml_modules
  sfml_deps += [dependency('sfml-' + mod)]
endforeach

spdlog_proj = subproject('spdlog')
spdlog_dep = spdlog_proj.get_variable('spdlog_dep')


exe = executable('mazetd', ['mazetd.cpp'],
  include_directories: include_directories('subprojects/entt/single_include/'),
  dependencies : sfml_deps + [spdlog_dep])

test('basic', exe)
