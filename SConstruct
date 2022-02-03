###library list
libraries = ['irrlicht']
pathToLibraries=['./lib']

env = Environment(
    CPPPATH=['lib/','./src/utils/']
)

optimize = ARGUMENTS.get('optimize',0)

if optimize == 1:
    env.Append(CCFLAGS='-O3 -std=c++20')
else:
    env.Append(CCFLAGS='-g -std=c++20 -Wall -Wpedantic -Wconversion')


app = env.Program(target= 'randysEngine',source = 'src/main.cpp',LIBS = libraries, LIBPATH=pathToLibraries )
#Library(target= 'randysEngine',source = src_files)
