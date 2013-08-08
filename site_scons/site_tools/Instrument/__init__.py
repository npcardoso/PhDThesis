import os
from SCons.Script import *
from SCons.Builder import Builder
from SCons.Scanner.C import CScanner

class ToolInstrumentWarning(SCons.Warnings.Warning):
    pass

class MissingRequiredVariable(ToolInstrumentWarning):
    def __init__(self, var):
        super(ToolInstrumentWarning, self).__init__("Missing required variable '%s'" % var)

class AppNotFound(ToolInstrumentWarning):
    def __init__(self, app):
        super(ToolInstrumentWarning, self).__init__("Could not find application '%s'" % app)


def _detect_app(env, var_name, app_name):
    """ Try to detect a specific application """
    try:
        return env[var_name]
    except KeyError:
        pass

    app = env.WhereIs(app_name)
    if app:
        return app

    raise SCons.Errors.StopError(AppNotFound(app_name))
    return None


_instrument_llvm_builder_c = SCons.Builder.Builder(
    action = SCons.Action.Action('$CLANG -S -emit-llvm -g $CFLAGS $CPP_PATH -c $SOURCE -o $TARGET'),
    src_suffix = '$INSTR_SOURCE_SUFFIX_C',
    suffix = '$INSTR_LLVM_SUFFIX',
    single_source = 1)

_instrument_llvm_builder_cpp = SCons.Builder.Builder(
    action = SCons.Action.Action('$CLANGPP -S -emit-llvm -g $CFLAGS $CPP_PATH -c $SOURCE -o $TARGET'),
    src_suffix = '$INSTR_SOURCE_SUFFIX_CPP',
    suffix = '$INSTR_LLVM_SUFFIX',
    single_source = 1)

_instrument_llvm_instrument_builder = SCons.Builder.Builder(
    action = SCons.Action.Action('$OPT -S -f --load $LLVM_INSTRUMENT_LIB $LLVM_INSTRUMENT_PASSES $SOURCE -o $TARGET'),
    src_suffix = '$INSTR_LLVM_SUFFIX',
    suffix = '$INSTR_LLVM_INSTR_SUFFIX',
    single_source = 1)

_instrument_asm_builder = SCons.Builder.Builder(
    action = SCons.Action.Action('$LLC -o $TARGET < $SOURCE'),
    src_suffix = '$INSTR_LLVM_INSTR_SUFFIX',
    suffix = '$INSTR_ASM_SUFFIX',
    single_source = 1)

_instrument_linker_builder = SCons.Builder.Builder(
    action = SCons.Action.Action('$CLANGPP $LINK_PATH $LINKFLAGS -o $TARGET $SOURCES'),
    src_suffix = '$INSTR_ASM_SUFFIX')

def RecursiveScanner(scanner, file):
    #Scan header include tree
    print file.abspath, os.path.exists(file.srcnode().abspath)
    if not os.path.exists(file.srcnode().abspath):
        return []
    tmp = scanner(file)
    header_depends = set()
    header_pool = set(tmp)
    while(len(header_pool)):
        tmp = header_pool.pop()
        header_depends.add(tmp)
        if not os.path.exists(tmp.srcnode().abspath):
            continue
        for dep in scanner(tmp):
            path = os.path.dirname(str(tmp))
            if path:
                header_pool.add(File(path + "/" + str(dep)))
            else:
                header_pool.add(dep)

        header_pool -= header_depends

    return list(header_depends)

def Instrument(env, target, source=None, passes="-instrument_prepare", *args, **kw):
    env['LLVM_INSTRUMENT_PASSES'] = passes

    source_suffixes = [[env.subst('$INSTR_SOURCE_SUFFIX_C'), _instrument_llvm_builder_c],
                       [env.subst('$INSTR_SOURCE_SUFFIX_CPP'), _instrument_llvm_builder_cpp]]

    llvm_suffix = env.subst('$INSTR_LLVM_SUFFIX')
    llvm_instr_suffix = env.subst('$INSTR_LLVM_INSTR_SUFFIX')
    asm_suffix = env.subst('$INSTR_ASM_SUFFIX')

    llvm_instrument_lib = env.subst('$LLVM_INSTRUMENT_LIB')
    instrument_lib = env.subst('$INSTRUMENT_LIB')

    scanner = CScanner()
    result = []
    asm_objects = []
    for s in source:
        stem = src = str(s)
        if(not isinstance(s, SCons.Node.FS.File)):
            s = File(s)
        if not os.path.exists(s.srcnode().abspath):
            continue

        suffix = None
        for suf, fun in source_suffixes:
            if src.endswith(suf):
                stem = src[:-len(suf)]
                llvm_obj = fun.__call__(env, stem + llvm_suffix, src, **kw)
                suffix = suf
                break
        if suffix is None:
            raise Exception("Unknown filetype (%s)" % src)


        llvm_instrument_obj = _instrument_llvm_instrument_builder.__call__(env,
                                                                           stem + llvm_instr_suffix,
                                                                           stem + llvm_suffix,
                                                                           **kw)

        asm_obj = _instrument_asm_builder.__call__(env,
                                                   stem + asm_suffix,
                                                   stem + llvm_instr_suffix,
                                                   **kw)
        #Scan header include tree
        env.Depends(llvm_obj, RecursiveScanner(scanner.scan, s))

        env.Depends(llvm_instrument_obj, llvm_obj)
        env.Depends(llvm_instrument_obj, llvm_instrument_lib)
        env.Depends(asm_obj, llvm_instrument_obj)

        asm_objects.extend(asm_obj)

    env.Requires(target, instrument_lib)
    target_obj = _instrument_linker_builder.__call__(env, target, asm_objects, **kw)

    return target_obj


_required_vars = ['LLVM_INSTRUMENT_LIB',
                  'INSTRUMENT_LIB']

_instrument_apps = [('CLANG', 'clang'),
                    ('OPT', 'opt'),
                    ('LLC', 'llc'),
                    ('CLANGPP', 'clang++')]
def generate(env):
    """Add Builders and construction variables to the Environment."""
    exists(env)

    for var_name, app_name in _instrument_apps:
        if(var_name in env):
            env[var_name] = _detect_app(env, var_name, env[var_name])
        else:
            env[var_name] = _detect_app(env, var_name, app_name)

    env.SetDefault(
        # Suffixes/prefixes
        INSTR_SOURCE_SUFFIX_C = '.c',
        INSTR_SOURCE_SUFFIX_CPP = '.cpp',
        INSTR_LLVM_SUFFIX = '.bc',
        INSTR_LLVM_INSTR_SUFFIX = '.bci',
        INSTR_ASM_SUFFIX = '.s')
    env.AddMethod(Instrument, "Instrument");



def exists(env):
    for var_name in _required_vars:
        if var_name not in env:
            raise SCons.Errors.StopError(MissingRequiredVariable(var_name))
    for var_name, app_name in _instrument_apps:
        _detect_app(env, var_name, app_name)
