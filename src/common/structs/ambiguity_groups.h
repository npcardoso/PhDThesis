#ifndef __AMBIGUITY_GROUPS_H_8ee792cbc6e3c1036d8a9adecc7390d05b48e1b6__
#define __AMBIGUITY_GROUPS_H_8ee792cbc6e3c1036d8a9adecc7390d05b48e1b6__

#include "structs/spectra.h"
#include "types.h"

#include <cstring>
#include <iomanip>
#include <map>
#include <set>

namespace diagnosis {
namespace structs {
class t_ambiguity_groups {
public:
    typedef std::set<t_component_id> t_group;

    t_ambiguity_groups ();
    t_ambiguity_groups (const t_spectra & spectra,
                        const t_spectra_filter * filter=NULL);

    void iterator (t_spectra_iterator & it) const;
    const t_spectra_filter & filter () const;

    const t_group * group(t_component_id) const;

    inline t_count get_component_count () const {return component_count;}
    inline t_count get_transaction_count () const {return transaction_count;}

private:
    typedef std::map<t_component_id, t_group> t_group_map;
    t_group_map groups;
    t_spectra_filter _filter;
    t_count component_count,
            transaction_count;
};
}
}

namespace std {
ostream & operator << (ostream & out, const diagnosis::structs::t_ambiguity_groups & ag);
}

#endif