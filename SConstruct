build_dir = 'obj'

config = {}
Export('config')
config['build_name'] = "a.out"
config['c++'] = 'clang++'
config['mpi_cflags'] = '`mpic++ --showme:compile`'
config['mpi_lflags'] = '`mpic++ --showme:link`'



config['cflags'] = '-pg'
config['lflags'] = ''
SConscript('SConscript', variant_dir=build_dir + "/test")

config['cflags'] = '-DNDEBUG'
config['lflags'] = ''
SConscript('SConscript', variant_dir=build_dir + "/performance")
