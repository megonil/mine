subdir('lexer')
subdir('parser')
subdir('compiler')
subdir('error')

executable(
	'minec',
	'main.cpp',
	include_directories: includes,
	link_with: [lexer_lib],
	install: true,
	dependencies: deps,
)
