import os

#Import("build_info")

root = Dir('#').abspath
#common_include_dir = root + "/include"

target  = "a.out"
sources = Glob("*cpp") + Glob("*/*cpp")

env = Environment(
	CCFLAGS = "-O0 -g -std=c++0x",
	LDFLAGS = "-g -lpthread")


env['ENV']['TERM'] = os.environ['TERM']
env["CXX"] = "clang++"

obj = env.Program(target = target, source = sources)
#lib_install = env.Install(build_info['build_dir'] + "/lib", obj)

env.Default(obj)
