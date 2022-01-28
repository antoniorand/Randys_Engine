###library list
libraries = ['irrlicht']
pathToLibraries=['./lib']

env = Environment(
    CPPPATH=['lib/','./src/utils/'],
    SCONS_CXX_STANDARD='c++20',
)


app = env.Program(target= 'randysEngine',source = 'src/main.cpp',LIBS = libraries, LIBPATH=pathToLibraries )
#Library(target= 'randysEngine',source = src_files)
