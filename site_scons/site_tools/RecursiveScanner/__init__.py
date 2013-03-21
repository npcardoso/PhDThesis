import os
from SCons import Environment
def generate(env):
    """Add Builders and construction variables to the Environment."""
    env.AddMethod(RecursiveScanner, "RecursiveScanner");

def exists(env):
    return True

def RecursiveScanner(env, scanner, file, *args,**kargs):
    #Scan header include tree
    tmp = scanner(file, *args, **kargs)
    header_depends = set()
    header_pool = set(tmp)
    while(len(header_pool)):
        tmp = header_pool.pop()
        header_depends.add(tmp)
        if os.path.exists(tmp.abspath):
            print("File not found %s" % tmp.abspath)
        for dep in scanner(tmp, *args, **kargs):
            path = os.path.dirname(tmp.abspath)
            if path:
                header_pool.add(env.File(os.path.join(path, str(dep))))
            else:
                header_pool.add(dep)

        header_pool -= header_depends
        
    return list(header_depends)
