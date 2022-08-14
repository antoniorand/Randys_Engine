
env = Environment()

if(env['PLATFORM'] == 'msys'):
    libraries = ['glfw3','gdi32','soil','opengl32']
else:
    libraries = ['glfw']

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

nintendo3dsBuild = ARGUMENTS.get('3dsBuild',0)
nintendo3dsClean = ARGUMENTS.get('3dsClean',0)

if int(nintendo3dsBuild):
    env.Execute('make')
    Exit(1)
elif int(nintendo3dsClean):
    env.Execute('make clean')
    Exit(1)
else:
    env.Append(CPPPATH= 'lib/')
    #env.Replace(CXX = 'mingw-w64-gcc')
    optimize = ARGUMENTS.get('optimize',0)
    if int(optimize):
        env.Append(CCFLAGS='-O3 -std=c++20')
    else:
        env.Append(CCFLAGS='-g -std=c++20 -Wall -Wpedantic -Wconversion')
    asan = ARGUMENTS.get('clang-sanitize',0)
    clang = ARGUMENTS.get('clang',0)
    if int(clang):
        env.Replace(CC= "clang", CXX="clang++")
        #libraries.append('stdc++fs')
    if int(asan):
        env.Replace(CC= "clang", CXX="clang++")
        env.Replace(CCFLAGS='-O3 -std=c++20 ')
        env.Append(CCFLAGS='-fsanitize=address', LINKFLAGS='-fsanitize=address -fno-omit-frame-pointer')
    if(env['PLATFORM'] == 'msys'):
        env.Append(LINKFLAGS='-static -static-libgcc -static-libstdc++')
    app = env.Program(target= 'randysEngine',source = AllSources('./src', '*.cpp*'),LIBS = libraries, LIBPATH=pathToLibraries )
    #Library(target= 'randysEngine',source = src_files)