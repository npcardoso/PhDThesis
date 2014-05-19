#include "ambiguity_groups.h"

#include "utils/iostream.h"

#include <boost/uuid/sha1.hpp>

using boost::uuids::detail::sha1;
namespace diagnosis {
namespace structs {
std::string sha1_to_string (sha1 & s) {
    unsigned int digest[5];
    std::string hash(20, 0);


    s.get_digest(digest);

    for (int i = 0; i < 5; ++i) {
        const char * tmp = reinterpret_cast<char *> (digest);
        hash[i * 4] = tmp[i * 4 + 3];
        hash[i * 4 + 1] = tmp[i * 4 + 2];
        hash[i * 4 + 2] = tmp[i * 4 + 1];
        hash[i * 4 + 3] = tmp[i * 4];
    }

    return hash;
}

t_ambiguity_groups::t_ambiguity_groups () {
    component_count = 0;
    transaction_count = 0;
}

t_ambiguity_groups::t_ambiguity_groups (const t_spectra & spectra,
                                        const t_spectra_filter * f) {
    typedef std::map<std::string, t_component_id> t_shas_map;
    t_shas_map shas;
    t_spectra_iterator it(spectra.get_component_count(),
                          spectra.get_transaction_count(),
                          f);

    if (f)
        _filter = *f;

    component_count = spectra.get_component_count();
    transaction_count = spectra.get_transaction_count();

    while (it.component.next()) {
        sha1 s;
        t_component_id c_id = it.component.get();

        // Calculate SHA1
        while (it.transaction.next()) {
            t_count count = spectra.get_activations(c_id,
                                                    it.transaction.get());
            s.process_bytes(&count, sizeof(t_count));
        }


        std::string hash = sha1_to_string(s);

        // Check if is unique
        if (shas.count(hash)) {
            _filter.components.filter(c_id);
            groups[shas[hash]].insert(c_id);
        }
        else
            shas[hash] = c_id;
    }
}

void t_ambiguity_groups::iterator (t_spectra_iterator & it) const {
    it = t_spectra_iterator(component_count,
                            transaction_count,
                            &_filter);
}

const t_spectra_filter & t_ambiguity_groups::filter () const {
    return _filter;
}

const t_ambiguity_groups::t_group * t_ambiguity_groups::group (t_component_id c_id) const {
    t_group_map::const_iterator it = groups.find(c_id);


    if (it != groups.end())
        return &it->second;

    return NULL;
}
}
}

using namespace diagnosis::structs;

std::ostream & std::operator << (std::ostream & out, const t_ambiguity_groups & ag) {
    const t_spectra_filter & f = ag.filter();

    t_component_id next_group = 0;


    while (true) {
        next_group = f.components.next(next_group);

        if (next_group > ag.get_component_count())
            break;

        const t_ambiguity_groups::t_group * g = ag.group(next_group);

        if (g)
            out << "g(" << next_group << ") = " << *g << std::endl;
    }

    return out;
}