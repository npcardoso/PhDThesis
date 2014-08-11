#ifndef __IOSTREAM_H_6e998e029b3b9c5f4d1871ad32300819656f530f__
#define __IOSTREAM_H_6e998e029b3b9c5f4d1871ad32300819656f530f__

#include <iostream>
#include <list>
#include <map>
#include <set>
#include <vector>
namespace std {
template <class C, class D>
std::ostream & operator << (std::ostream & out, const std::pair<C, D> & p) {
    return out << "p(" << p.first << ',' << p.second << ')';
}

template <class C>
std::ostream & printContainer (std::ostream & out, const C & container, std::string lbracket="", std::string rbracket="") {
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

template <class C>
std::ostream & operator << (std::ostream & out, const std::set<C> & container) {
    return printContainer(out, container, "s(", ")");
}

template <class K, class V>
std::ostream & operator << (std::ostream & out, const std::map<K, V> & container) {
    return printContainer(out, container, "m(", ")");
}

template <class C>
std::ostream & operator << (std::ostream & out, const std::vector<C> & container) {
    return printContainer(out, container, "v[", "]");
}

template <class C>
std::ostream & operator << (std::ostream & out, const std::list<C> & container) {
    return printContainer(out, container, "l[", "]");
}
}
#endif