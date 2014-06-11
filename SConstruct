import os
from os.path import join

env = Environment()


root = '#'


vars = Variables('.scons.conf')

vars.Add('PATH', '', '')
vars.Add('LIBPATH', '', '')
vars.Add('CPPPATH', '', '')

vars.Add('prefix', '', join(root, 'obj', 'install'))
vars.Add('build_dir', '', join(root, 'obj'))
vars.Add('libext', '', 'so')

vars.Add('libinstrument', '', 'instrument')
vars.Add('llvminstrument', '', 'llvminstrument')

vars.Add('debug', '', False)
vars.Add('colors', '', True)

vars.Add('CLANG', '', 'clang')
vars.Add('CLANGPP', '', 'clang++')
vars.Add('llvmconfig', '', 'llvm-config')
vars.Add('OPT', '', 'opt')
vars.Add('LLC', '', 'llc')
vars.Add('LLI', '', 'lli')
vars.Add('CXX', '', 'clang++')




vars.Update(env)

if env['colors']:
    ## Scons output colorizerdd
    ## cf. http://code.google.com/p/scolorizer/wiki/GettingStarted
    try:
        from colorizer import colorizer
        colorizer().colorize(env)
        scolorizer_available = True
    except ImportError:
        pass






#Install/Build Dirs
for i in ['prefix', 'build_dir']:
    env[i]  = env.Dir(env[i]).abspath

for i in ['include', 'lib', 'bin']:
    env['%s_dir' % i] = join(env['prefix'], i)

for i in ['PATH', 'LIBPATH', 'CPPPATH']:
    if not isinstance(env[i], list):
      env[i] = [env[i]]
env['PATH'].extend(['/usr/local/bin', '/bin', '/usr/bin'])

env['common_dir'] = join(root, "src", "common")



#Build Flags

if(env['debug']):
    env['CCFLAGS'] = "-g -O0"
else:
    env['CCFLAGS'] = "-O3 -DNDEBUG"
env['CCFLAGS'] += " -std=c++11  -Wall -pedantic"

env['ENV']['TERM'] = os.environ['TERM']
env['ENV']['PATH'] = env['PATH']
env['ENV']['LD_LIBRARY_PATH'] = env['LIBPATH']


Export('env')
SConscript('SConscript', variant_dir=env['build_dir'])
