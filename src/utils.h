#ifndef __UTILS_H__
#define __UTILS_H__

#include <iostream>
#include <list>
#include <map>
#include <set>
#include <vector>

#include <sys/time.h>

#define iterators(C) (C).begin(), (C).end()
#define riterators(C) (C).rbegin(), (C).rend()

#define foreach(var, container) \
    for(typeof((container).begin()) var = (container).begin(); \
            var != (container).end(); \
            ++var)


typedef long double time_interval_t;
inline time_interval_t get_time_interval(){
  timeval time;
  gettimeofday(&time, NULL);
  return time.tv_sec + time.tv_usec / 1e6;
}

template <class C, class D> std::ostream & operator << (std::ostream & out, const std::pair<C, D> & p) {
    return out << '(' << p.first << ',' << p.second << ')';
}

template <class C> std::ostream & printContainer(std::ostream & out, const C & container, std::string lbracket="", std::string rbracket="") {
    out << lbracket;
    if (container.size()) {
        out << *(container.begin());
        typename C::const_iterator i = container.begin();

        while (++i != container.end())
            out << ", " << *i;
    }
    out << rbracket;
    return out;
}

template <class C> std::ostream & operator << (std::ostream & out, const std::set<C> & container) {
    return printContainer(out, container, "(", ")");
}
template <class K, class V> std::ostream & operator << (std::ostream & out, const std::map<K, V> & container) {
    return printContainer(out, container, "m(", ")");
}
template <class C> std::ostream & operator << (std::ostream & out, const std::vector<C> & container) {
    return printContainer(out, container, "v[", "]");
}
template <class C> std::ostream & operator << (std::ostream & out, const std::list<C> & container) {
    return printContainer(out, container, "l[", "]");
}

#endif
