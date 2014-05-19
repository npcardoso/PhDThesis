#ifndef __TYPES_H_71775dd5c74711d69f08402b7111a6e6d5056c07__
#define __TYPES_H_71775dd5c74711d69f08402b7111a6e6d5056c07__

#define EPSILON 1e-10

typedef double t_error;
typedef double t_health;
typedef double t_confidence;
typedef unsigned short t_port_no;

/* IDs */

typedef int t_sid;

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
typedef double t_score;
typedef double t_probability;
typedef double t_weight;
typedef double t_density;

//#include "utils/mpreal.h"
#include <boost/multiprecision/mpfr.hpp>

// Arbitrary Precision
namespace mp = boost::multiprecision;
typedef mp::number<mp::mpfr_float_backend<300>, mp::et_off>  t_probability_mp;
typedef t_probability_mp t_goodness_mp;

// Double Precision
typedef t_probability t_goodness;

#endif
