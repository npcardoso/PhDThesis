#ifndef __SERVER_SERVICES_JSON_H__
#define __SERVER_SERVICES_JSON_H__

#include "../service.h"
#include "../../../types.h"

#include <boost/property_tree/ptree.hpp>

class t_json_service {
public:
    virtual void operator () (std::istream & in,
                              std::ostream & out,
                              const boost::property_tree::ptree & pt) = 0;
};


class t_json_adapter_service : public t_service {
private:
    t_ptr<t_json_service> srv;
public:
    t_json_adapter_service (const t_ptr<t_json_service> & srv);

    virtual void operator () (std::istream & in,
                              std::ostream & out);
};

#endif