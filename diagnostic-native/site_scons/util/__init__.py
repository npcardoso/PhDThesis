from os.path import join

def findFiles(env, root, expr, ndirs):
    # Collect files
    wildcards = root
    ret = []

    for i in range(ndirs):
        wildcards = join(wildcards, "*")
        ret += env.Glob(wildcards + expr)

    return ret
