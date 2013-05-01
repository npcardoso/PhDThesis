from os.path import join

Import('env')
env = env.Clone()

env.SConscript('common/SConscript')
env.SConscript('instrumentation/llvm/SConscript')
env.SConscript('instrumentation/lib/SConscript')
env.SConscript('tools/SConscript')
env.SConscript('sandbox/SConscript')

env.Append(
    LLVM_INSTRUMENT_LIB = join(env['lib_dir'], "lib%s.%s" % (env['llvminstrument'], env['libext'])),
    INSTRUMENT_LIB = join(env['lib_dir'], "lib%s.%s" % (env['libinstrument'], env['libext'])),
    LINK_PATH = "-L%s" % env['lib_dir'],
    LINKFLAGS = "-l%s" % env['libinstrument'],
	CPP_PATH = "-I%s" % env['include_dir'])
env.Tool('Instrument')

env.Export('env')
env.SConscript('examples/fractal/SConscript')
env.SConscript('examples/webfs/SConscript')
