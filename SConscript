Import('env')

env.SConscript('src/common/SConscript')
env.SConscript('src/instrumentation/SConscript')
env.SConscript('src/diagnosis/SConscript')

if env['default_libRdiag']:
  env.SConscript('tools/libRdiag/SConscript')
