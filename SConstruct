import os

build_dir = 'obj'

config = {}
Export('config')
config['build_name'] = "a.out"
config['mpi_cflags'] = '`mpic++ --showme:compile`'
config['mpi_lflags'] = '`mpic++ --showme:link`'

cflags = "-Wall -ansi -pedantic"
env = Environment()

config['c++'] = 'g++'
config['cflags'] = '%s -O0 -pg -g' % cflags
config['lflags'] = '-pg'
config['target'] = 'debug'
config['default'] = False
env.SConscript('SConscript', variant_dir=build_dir + '/' + config['target'])

if 'CXX' in os.environ:
  config['c++'] = os.environ['CXX']
else:
  config['c++'] = 'g++'
config['cflags'] = '%s -DNDEBUG -O3' % cflags
config['lflags'] = '-O3'
config['target'] = 'performance'
config['default'] = True
env.SConscript('SConscript', variant_dir=build_dir + '/' + config['target'])

