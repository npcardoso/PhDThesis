build_dir = 'obj'

config = {}
Export('config')
config['build_name'] = "a.out"
config['c++'] = 'clang++'
config['mpi_cflags'] = '`mpic++ --showme:compile`'
config['mpi_lflags'] = '`mpic++ --showme:link`'

cflags = "-Wall -ansi -pedantic"

config['cflags'] = '%s -pg -g' % cflags
config['lflags'] = ''
SConscript('SConscript', variant_dir=build_dir + "/test")

config['cflags'] = '%s -DNDEBUG -O3' % cflags
config['lflags'] = '-O3'
SConscript('SConscript', variant_dir=build_dir + "/performance")
