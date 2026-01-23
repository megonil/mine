project(
	'mine',
	'cpp',
	'c',
	version: '0.1',
	default_options: ['cpp_std=c++23'],
)

llvm = dependency('llvm')
deps = [llvm]

inc_dirs = [
	'includes',
	'includes/lexer',
	'includes/parser',
	'includes/compiler',
	'includes/error',
	'includes/value',
]

includes = include_directories(inc_dirs)

subdir('src')
