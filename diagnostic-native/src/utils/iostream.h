#ifndef __IOSTREAM_H_6e998e029b3b9c5f4d1871ad32300819656f530f__
#define __IOSTREAM_H_6e998e029b3b9c5f4d1871ad32300819656f530f__

#include <iostream>
#include <list>
#include <map>
#include <set>
#include <vector>
#include <stack>

/**
 * This is a helper class for outputting lists/objects/etc...
 * The use of nested groups is supported by this class.
 */
class t_group {
public:
    /**
     * @brief Opens a group.
     */
    std::ostream & open (std::ostream & out,
                         std::string lbracket,
                         std::string rbracket,
                         std::string sep=",");

    /**
     * @brief This function should be called before each element of the group.
     * This function adds a separator before each element except for the first one.
     */
    std::ostream & element (std::ostream & out);

    /**
     * @brief Closes a group.
     */
    std::ostream & close (std::ostream & out);

private:
    class t_stack_element {
public:
        t_stack_element (std::string rbracket,
                         std::string sep);

        bool first;
        std::string rbracket;
        std::string sep;
    };

    std::stack<t_stack_element> s;
};


namespace std {
template <class C, class D>
std::ostream & operator << (std::ostream & out,
                            const std::pair<C, D> & p) {
    return out << p.first << ':' << p.second;
}

template <class C>
std::ostream & writeContainer (std::ostream & out,
                               const C & container,
                               std::string lbracket="",
                               std::string rbracket="") {
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
std::ostream & operator << (std::ostream & out,
                            const std::set<C> & container) {
    return writeContainer(out, container, "(", ")");
}

template <class K, class V>
std::ostream & operator << (std::ostream & out,
                            const std::map<K, V> & container) {
    return writeContainer(out, container, "{", "}");
}

template <class C>
std::ostream & operator << (std::ostream & out,
                            const std::vector<C> & container) {
    return writeContainer(out, container, "[", "]");
}

template <class C>
std::ostream & operator << (std::ostream & out,
                            const std::list<C> & container) {
    return writeContainer(out, container, "[", "]");
}
}
#endif