subdir('error')
subdir('lexer')
subdir('parser')
subdir('compiler')

executable(
	'minec',
	'main.cpp',
	include_directories: includes,
	link_with: [lexer_lib, parser_lib, compiler_lib],
	install: true,
	dependencies: deps,
)
