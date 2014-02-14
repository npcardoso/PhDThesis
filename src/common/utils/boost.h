#ifndef __UTILS_BOOST_H__
#define __UTILS_BOOST_H__

#include <boost/shared_ptr.hpp>
#include <boost/filesystem/path.hpp>

typedef boost::filesystem::path t_path;

template <class T>
class t_ptr : public boost::shared_ptr<T> {
    using boost::shared_ptr<T>::shared_ptr;
};

template <class T>
class t_const_ptr : public boost::shared_ptr<const T> {
    using boost::shared_ptr<const T>::shared_ptr;
};


#define DEFINE_BOOST_SHARED_PTRS(X)\
    typedef boost::shared_ptr<X> t_ptr;\
    typedef boost::shared_ptr<const X> t_const_ptr;

#endif