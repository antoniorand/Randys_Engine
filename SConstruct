env = Environment(
    CPPATH='',
    CPPDEFINES = [],
    SCONS_CXX_STANDARD='c++20'
)

###Lista de librerias
libraries = []
pathToLibraries=['.']

app = env.Program(target= 'randysEngine',source = 'src/main.cpp', LIBS = libraries, LIBPATH=pathToLibraries)
#Library(target= 'randysEngine',source = src_files)

for item in app:
    for child in item.all_children():
        print(child)