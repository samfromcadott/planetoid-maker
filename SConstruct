import os
from os.path import join, dirname
import shutil
import subprocess

# Web environment
target = 'wasm32-unknown-emscripten'

VariantDir('build/web', 'src', duplicate=False)
web_env = Environment(
	CC="emcc",
	CXX="em++",
	# TOOLS=TOOLS,
	CPPPATH=['include', 'submodule/raylib-cpp/include', 'submodule/tileson'],
	ENV = {'PATH' : os.environ['PATH']},
	LIBS=['raylib'],
	LIBPATH=['lib'],
	CCFLAGS = f'--target={target} -static -Wno-unknown-warning-option -Wunused-variable -O3',
	CXXFLAGS=f'--target={target} -static -std=c++20 -Wno-unknown-warning-option -Wunused-variable -O3 -Wno-unqualified-std-cast-call',
	LINKFLAGS=f'--target={target} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s ALLOW_MEMORY_GROWTH --shell-file shell.html',
	PROGSUFFIX='.html'
)
web_env['ENV']['TERM'] = os.environ['TERM'] # Colored output

# win_source = [ Glob('build/windows/*.cc') ]
web_source = [ Glob('build/web/*.cc') ]

# win_env.Program('bin/biogoth.exe', win_source)
web_env.Program('output/index', web_source)

# subprocess.call( [ '7za', 'u', 'biogoth.zip', 'assets', 'config.cfg', './bin/*' ] ) # Put the files in an archive
