import os
from os.path import join


root = '#'


vars = Variables('.scons.conf')

vars.Add('prefix', '', '/usr/local')
vars.Add('build_dir', '', join(root, 'obj'))
vars.Add('libext', '', 'so')

vars.Add('libinstrument', '', 'instrument')
vars.Add('llvminstrument', '', 'llvminstrument')

vars.Add('llvm_passes', '', '-instrument_function -instrument_prepare')

vars.Add('debug', '', False)

vars.Add('boost_lib', '', None)
vars.Add('boost_include', '', None)

vars.Add('clang', '', 'clang++')
vars.Add('llvmconfig', '', 'llvm-config')
vars.Add('OPT', '', 'opt')
vars.Add('LLC', '', 'llc')
vars.Add('LLI', '', 'lli')
vars.Add('CXX', '', 'g++')


env = Environment()

vars.Update(env)

env['STATIC_AND_SHARED_OBJECTS_ARE_THE_SAME']=0

env['prefix']  = env.Dir(env['prefix']).abspath
env['build_dir']  = env.Dir(env['build_dir']).abspath

env['common_dir'] = join(root, "common")

env['include_dir'] = join(env['prefix'], "include")
env['lib_dir'] = join(env['prefix'], "lib")
env['bin_dir'] = join(env['prefix'], "bin")

if 'boost_include' in env:
	env.Append(CPPPATH = [env['boost_include']])

if 'boost_lib' in env:
	env.Append(LIBPATH = [env['boost_lib']])

if(env['debug']):
  env['CCFLAGS'] = "-g -O0"
else:
  env['CCFLAGS'] = "-O3 -DNDEBUG"

env['CXX'] = env['clang']
env['ENV']['TERM'] = os.environ['TERM']

#vars.Save('.scons.conf', env)

Export('env')
SConscript('SConscript', variant_dir=env['build_dir'])
