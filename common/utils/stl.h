#ifndef __INSTR_STLUTILS_H__
#define __INSTR_STLUTILS_H__


#define iterators(C) (C).begin(), (C).end()

#define iteratorFor(C, N) typeof(C) N=(C)
#define foreach(var, container) \
    for(typeof((container).begin()) var = (container).begin(); \
            var != (container).end(); \
            ++var)

#endif
