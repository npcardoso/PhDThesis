#ifndef __UTILS_BOOST_H__
#define __UTILS_BOOST_H__

#include <boost/shared_ptr.hpp>

#define DEFINE_BOOST_SHARED_PTRS(X) \
  typedef boost::shared_ptr<X> t_ptr; \
  typedef boost::shared_ptr<const X> t_const_ptr;

#endif
