#ifndef __DIAGNOSIS_SPECTRA_AMBIGUITY_SIMPLIFY_H__
#define __DIAGNOSIS_SPECTRA_AMBIGUITY_SIMPLIFY_H__

#include "diagnosis/spectra.h"
#include "diagnosis/spectra_iterator.h"
#include "types.h"

#include <cstring>
#include <iomanip>
#include <map>
#include <set>

namespace diagnosis {
class t_ambiguity_groups {
public:
    typedef std::set<t_component_id> t_group;

    t_ambiguity_groups (const t_spectra & spectra,
                        const t_spectra_filter * filter=NULL);

    void iterator (t_spectra_iterator & it) const;
    const t_spectra_filter& filter () const;

    const t_group & group(t_component_id) const;

private:
    typedef std::map<t_component_id, t_group> t_group_map;
    t_group_map groups;
    t_spectra_filter _filter;
    t_count component_count,
            transaction_count;
};
}

#endif