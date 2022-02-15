###library list
libraries = ['irrlicht']
pathToLibraries=['./lib']

##Reference: https://stackoverflow.com/questions/26342109/scons-build-all-sources-files-in-all-directories

def AllSources(node='.', pattern='*'):
    result = [AllSources(dir, pattern)
              for dir in Glob(str(node)+'/*')
              if dir.isdir()]
    result += [source
               for source in Glob(str(node)+'/'+pattern)
               if source.isfile()]
    return result

env = Environment(
    CPPPATH=['lib/','./src/utils/']
)

#env.Replace(CXX = 'mingw-w64-gcc')

optimize = ARGUMENTS.get('release',0)

if optimize == 1:
    env.Append(CCFLAGS='-O3 -std=c++20')
else:
    env.Append(CCFLAGS='-ggdb3 -std=c++20 -Wall -Wpedantic -Wconversion')

env.Append(LINKFLAGS='-static -static-libgcc -static-libstdc++')

app = env.Program(target= 'randysEngine',source = AllSources('./src', '*.cpp*'),LIBS = libraries, LIBPATH=pathToLibraries )
#Library(target= 'randysEngine',source = src_files)
