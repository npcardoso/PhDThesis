import os
from os.path import join
import subprocess

env = Environment()
env['ENV']['TERM'] = os.environ['TERM']

vars = Variables('.scons.conf')
vars.Add('PATH', '', '')
vars.Add('LIBPATH', '', '')
vars.Add('CPPPATH', '', '')

vars.Add('prefix', '', '#/install/')
vars.Add('build_dir', '', '#/build/')

vars.Add('debug', '', False)
vars.Add('colors', '', True)
vars.Update(env)


if env['colors']:
    try:
        from colorizer import colorizer
        colorizer().colorize(env)
    except ImportError:
        pass


# Install/Build Dirs
for i in ['prefix', 'build_dir']:
    env[i]  = env.Dir(env[i]).abspath
for i in ['include', 'lib', 'bin']:
    env['%s_dir' % i] = join(env['prefix'], i)

# Paths
for i in ['PATH', 'LIBPATH', 'CPPPATH']:
    if not isinstance(env[i], list):
      env[i] = [env[i]]
env['PATH'].extend(['/usr/local/bin', '/bin', '/usr/bin'])
env['ENV']['PATH'] = env['PATH']
env['ENV']['LD_LIBRARY_PATH'] = env['LIBPATH']


# Build Flags
if(env['debug']):
    env['CCFLAGS'] = "-g -O0"
else:
    env['CCFLAGS'] = "-O3 -DNDEBUG"
env['CCFLAGS'] += " -std=c++11  -Wall -pedantic"

Export('env')
SConscript('SConscript', variant_dir=env['build_dir'])
