import os
from os.path import join

root = Dir('#').abspath


vars = Variables('.scons.conf')

vars.Add('prefix', '', '/usr/local')
vars.Add('build_dir', '', join(root, 'obj'))
vars.Add('libext', '', 'so')

vars.Add('libinstrument', '', 'instrument')
vars.Add('llvminstrument', '', 'llvminstrument')

vars.Add('llvm_passes', '', '-instrument_function -instrument_prepare')

vars.Add('debug', '', False)

vars.Add('boost', '', None)
vars.Add('clang', '', 'clang++')
vars.Add('llvmconfig', '', 'llvm-config')
vars.Add('opt', '', 'opt')
vars.Add('llc', '', 'llc')
vars.Add('lli', '', 'lli')
vars.Add('cxx', '', 'g++')


env = Environment()

vars.Update(env)

env['prefix']  = env.Dir(env['prefix']).abspath
env['build_dir']  = env.Dir(env['build_dir']).abspath

env['common_include_dir'] = join(root, "common", "include")

env['include_dir'] = join(env['prefix'], "include")
env['lib_dir'] = join(env['prefix'], "lib")
env['bin_dir'] = join(env['prefix'], "bin")

if(env['debug']):
  env['CCFLAGS'] = "-g -O0"
else:
  env['CCFLAGS'] = "-O3 -DNDEBUG"

env['CXX'] = env['clang']
env['ENV']['TERM'] = os.environ['TERM']

#vars.Save('.scons.conf', env)

Export('env')
SConscript('SConscript', variant_dir=env['build_dir'])
