project('hecho', 'c', 'cpp',
	version: '1.0.0',
	meson_version: '>= 0.55.0',
	default_options: ['c_std=c99']
)

cc = meson.get_compiler('c')

# Add any C compiler flags as needed
project_c_args = []
possible_c_args = [
]
foreach arg: possible_c_args
	if cc.has_multi_arguments(arg)
		project_c_args += arg
	endif
endforeach
add_project_arguments(project_c_args, language: ['c', 'cpp'])

# Figure out what GIT version information we have
commit = run_command('git', 'log', '--pretty=format:%h', '-n', '1', meson.source_root())
if commit.returncode() != 0
        commit_number = 'trunk'
else
        commit_number = commit.stdout().strip()
endif
add_project_arguments('-DCOMMIT_NUMBER="@0@"'.format(commit_number), language: ['c', 'cpp'])
add_project_arguments('-DPROJECT_VERSION="@0@"'.format(meson.project_version()), language: ['c', 'cpp'])

# Define files to compile for build
source_files = files(
	'src/hecho.c'
)

include_dirs = include_directories('.', 'src')

# Define our build
hecho = executable('hecho', source_files)

