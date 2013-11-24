import os
from os.path import join
from subprocess import check_output


root = '#'


vars = Variables('.scons.conf')

vars.Add('PATH', '', '')
vars.Add('LIBPATH', '', '')
vars.Add('CPPPATH', '', '')

vars.Add('prefix', '', join(root, 'obj', 'install'))
vars.Add('build_dir', '', join(root, 'obj'))
vars.Add('libext', '', 'so')

vars.Add('libdiag', '', 'diag')
vars.Add('libinstrument', '', 'instrument')
vars.Add('llvminstrument', '', 'llvminstrument')

vars.Add('debug', '', False)

vars.Add('R_include', '', '/usr/include/R')

vars.Add('llvm_passes', '', '-instrument_function -instrument_prepare')
vars.Add('CLANG', '', 'clang')
vars.Add('CLANGPP', '', 'clang++')
vars.Add('llvmconfig', '', 'llvm-config')
vars.Add('OPT', '', 'opt')
vars.Add('LLC', '', 'llc')
vars.Add('LLI', '', 'lli')
vars.Add('CXX', '', 'clang++')


vars.Add('mpi_include', '', check_output(['mpic++', '--showme:compile']).strip())
vars.Add('mpi_link', '', check_output(['mpic++', '--showme:link']).strip())

vars.Add('mpfr_link', '', '-lgmp -lmpfr')

### Defaults ###
vars.Add('default_instrumentation_examples', '', True)
vars.Add('default_instrumentation', '', True)
vars.Add('default_libRdiag', '', True)

env = Environment()

vars.Update(env)
env['STATIC_AND_SHARED_OBJECTS_ARE_THE_SAME']=0

#Install/Build Dirs
for i in ['prefix', 'build_dir']:
    env[i]  = env.Dir(env[i]).abspath

for i in ['include', 'lib', 'bin']:
    env['%s_dir' % i] = join(env['prefix'], i)

for i in ['PATH', 'LIBPATH', 'CPPPATH']:
    if env[i] and not isinstance(env[i], list):
      env[i] = [env[i]]

env['common_dir'] = join(root, "src", "common")



#Build Flags

if(env['debug']):
    env['CCFLAGS'] = "-g -O0"
else:
    env['CCFLAGS'] = "-O3 -DNDEBUG"

env['ENV']['TERM'] = os.environ['TERM']


Export('env')
SConscript('SConscript', variant_dir=env['build_dir'])
