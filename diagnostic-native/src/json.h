#ifndef __JSON_H_ec30169a03d891d55545f6d26609a2cad278e208__
#define __JSON_H_ec30169a03d891d55545f6d26609a2cad278e208__

#include <iostream>

#include "candidate.h"
#include "diagnostic_report.h"
#include "structs/trie.h"

#include <boost/foreach.hpp>

namespace diagnostic{

#define CANDIDATE_JSON_PREFIX "["
#define CANDIDATE_JSON_SUFFIX "]"
#define CANDIDATE_JSON_SEP ", "
#define TRIE_JSON_PREFIX "["
#define TRIE_JSON_SUFFIX "]"
#define TRIE_JSON_SEP ", "

std::ostream & json_write (std::ostream & out, const int & i) {
    out << i;
    return out;
}

std::ostream & json_write (std::ostream & out, const double & i) {
    out << i;
    return out;
}

std::ostream & json_write (std::ostream & out, std::string str) {
    // TODO: escape string
    out << '"' << str << '"';
    return out;
}

template <class C, class D>
std::ostream & json_write (std::ostream & out, const std::pair<C, D> & p) {
    json_write(out, p.first);
    out << ':';
    json_write(out, p.second);
    return out;
}

template <class C>
std::ostream & json_write_container (std::ostream & out,
                                     const C & container,
                                     std::string lbracket="",
                                     std::string rbracket="") {
    out << lbracket;

    if (container.size()) {
        json_write(out, *(container.begin()));
        typename C::const_iterator i = container.begin();

        while (++i != container.end()) {
            out << ", ";
            json_write(out, *i);
        }
    }

    out << rbracket;
    return out;
}

template <class C>
std::ostream & json_write (std::ostream & out, const std::set<C> & container) {
    return json_write_container(out, container, "[", "]");
}

template <class K, class V>
std::ostream & json_write (std::ostream & out, const std::map<K, V> & container) {
    return json_write_container(out, container, "{", "}");
}

template <class C>
std::ostream & json_write (std::ostream & out, const std::vector<C> & container) {
    return json_write_container(out, container, "[", "]");
}

template <class C>
std::ostream & json_write (std::ostream & out, const std::list<C> & container) {
    return json_write_container(out, container, "[", "]");
}




inline std::ostream & json_write (std::ostream & out,
                                  const t_trie & t) {
    return t.generic_write(out,
                           TRIE_JSON_PREFIX,
                           TRIE_JSON_SUFFIX,
                           TRIE_JSON_SEP,
                           CANDIDATE_JSON_PREFIX,
                           CANDIDATE_JSON_SUFFIX,
                           CANDIDATE_JSON_SEP);
}


inline std::ostream & json_write (std::ostream & out,
                                 const t_candidate & c) {
    return c.generic_write(out,
                           CANDIDATE_JSON_PREFIX,
                           CANDIDATE_JSON_SUFFIX,
                           CANDIDATE_JSON_SEP);
}

inline std::ostream & json_write (std::ostream & out,
                                  const t_connection & c) {
    out << "{from:" << c.get_from() << ", to:" << c.get_to() << "}";
    return out;
}



inline std::ostream & json_write (std::ostream & out,
                                  const t_diagnostic_report & dr) {
    bool first;

    out << "{gen_results: [" ;
    first = true;
    BOOST_FOREACH(const auto & result,
                  dr.get_generator_results()) {
        if(!first)
            out << ",";

        if(result.get() == NULL)
            out << "null";
        else
            json_write(out, *result);
        first = false;
    }
    out << "], ";

    out << "rank_results: [" ;
    first = true;
    BOOST_FOREACH(const auto & result,
                  dr.get_ranker_results()) {
        if(!first)
            out << ",";

        if(result.get() == NULL)
            out << "null";
        else
            json_write(out, *result);
        first = false;
    }
    out << "], ";

    out << "connections: [" ;
    first = true;
    BOOST_FOREACH(const auto & connection,
                  dr.get_diagnostic_system()->get_connections()) {
        if(!first)
            out << ",";

        json_write(out, connection);

        first = false;
    }
    out << "]}";


    return out;
}

}

#endif
