import os

#Import("build_info")

root = Dir('#').abspath
#common_include_dir = root + "/include"

target  = "a.out"
sources = Glob("src/*cpp")

env = Environment(
	CCFLAGS = "-O0 -g -std=c++0x",
	LINKFLAGS = "-g -lpthread -pthread -L/usr/lib/openmpi -lmpi_cxx -lmpi -ldl -lhwloc")


env['ENV']['TERM'] = os.environ['TERM']
env["CXX"] = "clang++"

obj = env.Program(target = target, source = sources)
#lib_install = env.Install(build_info['build_dir'] + "/lib", obj)

env.Default(obj)
