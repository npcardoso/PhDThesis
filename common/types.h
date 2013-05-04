#ifndef __TYPES_H__
#define __TYPES_H__

//#include <boost/shared_ptr.hpp>

typedef double t_health;
typedef double t_confidence;
typedef unsigned short t_port_no;

#define DEFINE_BOOST_SHARED_PTRS(X) \
  typedef boost::shared_ptr<X> t_ptr; \
  typedef boost::shared_ptr<const X> t_const_ptr;

/* IDs */

typedef unsigned int t_id;
typedef t_id t_component_id;
typedef t_id t_label_id;
typedef t_id t_transaction_id;
typedef t_id t_spectrum_id;
typedef t_id t_construct_id;
typedef t_id t_observation_id;

/* Counts */

typedef unsigned int t_count;
typedef t_count t_ns_count;

/* Statistics */

typedef double t_weight;
typedef double t_density;
#endif
