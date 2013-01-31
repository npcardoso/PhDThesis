build_dir = 'obj'

build_info = {}
build_info['build_name'] = "a.out"
build_info['build_dir'] = str(Dir('#').abspath) + '/' + build_dir
build_info['common_include_dir'] = build_info['build_dir'] + "/include"
build_info['clang'] = 'clang'

Export('build_info')

SConscript('SConscript', variant_dir=build_dir)
