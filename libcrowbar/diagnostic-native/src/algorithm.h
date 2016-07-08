#ifndef __ALGORITHM_H_07fc6e867f14cba7b349ae30f17f00a2d5aec09f__
#define __ALGORITHM_H_07fc6e867f14cba7b349ae30f17f00a2d5aec09f__

#include "utils/json.h"
#include <map>

namespace diagnostic {
class t_algorithm : public t_json_writable {
public:
    typedef std::map<std::string, std::string> t_configs;

    virtual std::string get_name () const = 0;
    virtual std::ostream & json (std::ostream & out) const;

protected:
    virtual void json_configs (t_configs & out) const = 0;
};
}

namespace std {
ostream & operator << (ostream & out,
                       const diagnostic::t_algorithm & a);
}


#endif