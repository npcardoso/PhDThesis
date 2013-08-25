#ifndef __TOPOLOGY_H_ba7162d9bafcf1d03023d9851ec73fa65ad270b2__
#define __TOPOLOGY_H_ba7162d9bafcf1d03023d9851ec73fa65ad270b2__

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
    t_fault (t_probability pass_prob,
             t_probability soft_prob,
             t_probability hard_prob,
             t_probability fail_prob);

    t_error gen_error (boost::random::mt19937 & gen) const;
    bool gen_failure (boost::random::mt19937 & gen) const;

    inline t_probability get_pass_prob () const {return pass_prob;}
    inline t_probability get_soft_prob () const {return soft_prob;}
    inline t_probability get_hard_prob () const {return hard_prob;}
    inline t_probability get_fail_prob () const {return fail_prob;}

private:
    t_probability pass_prob;
    t_probability soft_prob;
    t_probability hard_prob;
    t_probability fail_prob;
};

class t_link {
public:
    typedef std::pair<t_component_id, t_probability> t_sink;
    typedef std::vector<t_sink> t_sinks;

    inline t_link () {total = 0;}


    t_link & add_sink (t_component_id comp, t_probability prob);

    inline const t_sinks & get_sinks () const {return sinks;}

    t_probability get_probability (t_component_id comp) const;
    inline t_probability get_normalization_value () const {return total;}

    t_component_id gen_destination (boost::random::mt19937 & gen) const;

private:
    std::vector<t_sink> sinks;
    t_probability total;
};


class t_topology {
public:
    DEFINE_BOOST_SHARED_PTRS(t_topology);
    typedef std::list<t_link> t_interface;
    typedef std::set<t_component_id> t_components;


    t_topology & add_fault (t_component_id comp, const t_fault & f);
    t_topology & add_link (t_component_id comp, const t_link & l);
    t_topology & add_entry_point (t_component_id comp, t_probability prob);

    const t_interface * get_interface (t_component_id comp) const;
    const t_fault * get_fault (t_component_id comp) const;
    const t_components & get_components () const;

    std::ostream & graphviz (std::ostream & out) const;

    t_component_id gen_entry_point (boost::random::mt19937 & gen) const;

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

    t_link entry_points;
};
}
}

#endif