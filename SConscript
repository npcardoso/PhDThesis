Import('env')

env.SConscript('src/common/SConscript')
env.SConscript('src/diagnosis/SConscript')

#if env['default_instrumentation']:
#  env.SConscript('src/instrumentation/SConscript')

if env['default_libRdiag']:
  env.SConscript('tools/libRdiag/SConscript')
