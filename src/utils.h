#ifndef __UTILS_H__
#define __UTILS_H__

#define iterators(C) (C).begin(), (C).end()
#define riterators(C) (C).rbegin(), (C).rend()

#define foreach(var, container) \
    for(typeof((container).begin()) var = (container).begin(); \
            var != (container).end(); \
            ++var)
#endif
