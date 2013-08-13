#ifndef __DIAGNOSIS_STRUCTS_TOPOLOGY_H__
#define __DIAGNOSIS_STRUCTS_TOPOLOGY_H__

#include "diagnosis/types.h"

#include <list>
#include <map>
#include <boost/random/mersenne_twister.hpp>

namespace diagnosis {
namespace structs {
class t_fault {
public:
    t_fault ();
    t_fault (t_goodness goodness);
    t_fault (t_goodness goodness, t_error e_min, t_error e_max);

    bool gen_error (boost::random::mt19937 & gen) const;
    t_error gen_pass_value (boost::random::mt19937 & gen) const;
    t_error gen_error_value (boost::random::mt19937 & gen) const;
    bool can_pass () const;

    inline t_error get_emin () const {return e_min;}
    inline t_error get_emax () const {return e_max;}
    inline t_probability get_failure () const {return failure;}
    inline t_goodness get_goodness () const {return goodness;}

private:
    t_error e_min, e_max;
    t_probability failure;
    t_goodness goodness;
};

class t_link {
public:
    t_link (t_probability prob, t_component_id branch1, t_component_id branch2=0);

    t_component_id operator () (boost::random::mt19937 & gen) const;

    inline t_component_id get_branch1 () const {return branch1;}
    inline t_component_id get_branch2 () const {return branch2;}
    inline t_probability get_prob () const {return prob;}

private:
    t_probability prob;
    t_component_id branch1, branch2;
};


class t_topology {
public:
    typedef std::list<t_link> t_interface;


    t_topology & operator () (t_component_id comp, const t_fault & f);
    t_topology & operator () (t_component_id comp, const t_link & l);

    const t_interface * get_interface (t_component_id comp) const;
    const t_fault * get_fault (t_component_id comp) const;

    std::ostream & graphviz (std::ostream & out) const;

private:
    std::ostream & graphviz_component (std::ostream & out, t_component_id comp) const;
    std::ostream & graphviz_links (std::ostream & out, t_component_id comp) const;

    typedef std::list<t_interface> t_interfaces;
    typedef std::list<t_fault> t_faults;
    typedef std::set<t_component_id> t_components;
    typedef std::map<t_component_id, t_faults::iterator> t_fault_bind;
    typedef std::map<t_component_id, t_interfaces::iterator> t_interface_bind;

private:

    t_components components;
    t_interfaces interfaces;
    t_faults faults;
    t_interface_bind interface_bind;
    t_fault_bind fault_bind;
};
}
}

#endif