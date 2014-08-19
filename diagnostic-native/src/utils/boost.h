#ifndef __BOOST_H_e5b23933b134058de17a89db01ca1093cae61f78__
#define __BOOST_H_e5b23933b134058de17a89db01ca1093cae61f78__

#include <boost/shared_ptr.hpp>

template <class T>
using t_ptr = boost::shared_ptr<T>;
template <class T>
using t_const_ptr = boost::shared_ptr<const T>;

#include <ostream>

namespace std {
template<class T>
std::ostream& operator<<(std::ostream& s, const t_const_ptr<T>& c)
{
    if(c)
        s << *c;
    else
        s << "(null)";
    return s;
}
}

#include <boost/filesystem/path.hpp>

typedef boost::filesystem::path t_path;

#endif
