#ifndef __CONFIGURATION_CREATOR_H_97838ec99bf6c10ee18eb2d879a46fbb072796c1__
#define __CONFIGURATION_CREATOR_H_97838ec99bf6c10ee18eb2d879a46fbb072796c1__


#include "algorithm_creator.h"

#include <boost/property_tree/ptree.hpp>

namespace diagnostic {
namespace configuration {

t_const_ptr<t_diagnostic_system> construct_diagnostic_system (const boost::property_tree::ptree & pt,
                                                                  const t_algorithm_creator & ac = t_algorithm_creator::get_default());
}
}



#endif
