def gen_tree_full(cur, elems):
    ret = []

    if not elems:
        return ""

    for i in elems:
        tmp = set(elems)
        tmp.remove(i)
        tmp_cur = set(cur)
        tmp_cur.add(i)
        ret.append("\nchild { node {\set%s} %s}" % (str(tmp_cur), gen_tree_full(tmp_cur, tmp)))

    ret.reverse()

    return "".join(ret)

def gen_tree_trimmed(cur, elems):
    ret = []

    if not elems:
        return ""

    tmp = set(elems)

    for i in elems:
        tmp.remove(i)
        tmp_cur = set(cur)
        tmp_cur.add(i)
        ret.append("\nchild { node {\set%(set)s} %(rest)s }" % {"set": str(tmp_cur),
                                                              "rest":gen_tree_trimmed(tmp_cur, tmp)})

    ret.reverse()


    return "".join(ret)


print(gen_tree_full([],[1,2,3]))
#print(gen_tree_trimmed([],[1,2,3]))
