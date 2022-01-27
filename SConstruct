###library list
libraries = []
pathToLibraries=['./lib']

env = Environment(
    CPPATH=['lib'],
    CPPDEFINES = [],
    SCONS_CXX_STANDARD='c++20',
    LIBS = libraries, LIBPATH=pathToLibraries
)


app = env.Program(target= 'randysEngine',source = 'src/main.cpp', )
#Library(target= 'randysEngine',source = src_files)
