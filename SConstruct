###library list
libraries = ['irrlicht']
pathToLibraries=['./lib']

env = Environment(
    CPPPATH=['lib/','./src/utils/']
)

debug = ARGUMENTS.get('debug',0)

if debug == 0:
    env.Append(CCFLAGS='-O3 -std=c++20')
else:
    env.Append(CCFLAGS='-ggdb3 -std=c++20 -Wall -Wpedantic -Wconversion -fsanitize=undefined')


app = env.Program(target= 'randysEngine',source = 'src/main.cpp',LIBS = libraries, LIBPATH=pathToLibraries )
#Library(target= 'randysEngine',source = src_files)
