Import('env')

env.SConscript('src/common/SConscript')

env.Append(LINKFLAGS = " -lboost_date_time  -lboost_filesystem -Wl,-rpath,'$$ORIGIN/../lib' %s " % env['mpfr_link'] )
Export('env')


env.SConscript('src/diagnosis/SConscript')
env.SConscript('src/sandbox/SConscript')
env.SConscript('src/tests/SConscript')


#if env['default_instrumentation']:
#  env.SConscript('src/instrumentation/SConscript')

if env['default_libRdiag']:
  env.SConscript('tools/libRdiag/SConscript')
