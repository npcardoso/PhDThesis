#ifndef __DIAGNOSIS_STRUCTS_TOPOLOGY_H__
#define __DIAGNOSIS_STRUCTS_TOPOLOGY_H__

#include "diagnosis/types.h"
#include "utils/boost.h"

#include <list>
#include <map>
#include <vector>
#include <boost/random/mersenne_twister.hpp>

namespace diagnosis {
namespace structs {
class t_fault {
public:
    t_fault ();
    t_fault (t_goodness goodness, t_probability failure);
    t_fault (t_goodness goodness, t_probability failure, t_error e_min, t_error e_max);

    bool gen_error (boost::random::mt19937 & gen) const;
    bool gen_failure (boost::random::mt19937 & gen) const;

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
    typedef std::pair<t_component_id, t_probability> t_sink;
    typedef std::vector<t_sink> t_sinks;

    inline t_link () {total = 0;}

    t_component_id operator () (boost::random::mt19937 & gen) const;

    t_link & add_sink (t_component_id comp, t_probability prob);

    inline const t_sinks & get_sinks () const {return sinks;}
    inline t_probability get_normalization_value () const {return total;}

private:
    std::vector<t_sink> sinks;
    t_probability total;
};


class t_topology {
public:
    DEFINE_BOOST_SHARED_PTRS(t_topology);
    typedef std::list<t_link> t_interface;
    typedef std::set<t_component_id> t_components;


    t_topology & operator () (t_component_id comp, const t_fault & f);
    t_topology & operator () (t_component_id comp, const t_link & l);

    const t_interface * get_interface (t_component_id comp) const;
    const t_fault * get_fault (t_component_id comp) const;
    const t_components & get_components () const;

    std::ostream & graphviz (std::ostream & out) const;

private:
    std::ostream & graphviz_component (std::ostream & out, t_component_id comp) const;
    std::ostream & graphviz_links (std::ostream & out, t_component_id comp) const;

    typedef std::list<t_interface> t_interfaces;
    typedef std::list<t_fault> t_faults;
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