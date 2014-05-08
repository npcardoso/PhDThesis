#ifndef __UTILS_BOOST_H__
#define __UTILS_BOOST_H__

#include <boost/shared_ptr.hpp>
#include <boost/filesystem/path.hpp>

typedef boost::filesystem::path t_path;

template <class T>
using t_ptr = boost::shared_ptr<T>;
template <class T>
using t_const_ptr = boost::shared_ptr<const T>;
#endif