#ifndef __SIMILARITY_H_516cee6798dbd3feb110e449857eb823bcdf0260__
#define __SIMILARITY_H_516cee6798dbd3feb110e449857eb823bcdf0260__

#include "../diagnostic_system.h"
#include "../report/single_component_ranking.h"
#include "../structs/spectrum.h"
#include "../types.h"
#include "../utils/boost.h"

#include <vector>

namespace diagnostic {
namespace algorithms {
class t_similarity : public t_candidate_ranker {
public:
    virtual void operator () (const t_spectrum & spectrum,
                              const t_trie & D,
                              t_ret_type & probs,
                              const t_spectrum_filter * filter=NULL) const;


    /**
     * @brief Calculates rank for all components in the spectrum (does not sort nor normalize rank)
     * @note: Uses similarity_coefficient(const t_count n[2][2]);
     */
    virtual t_ptr<t_single_component_ranking> operator () (const t_spectrum & spectrum,
                                                           const t_spectrum_filter * filter=NULL) const;


    /**
     * @brief Calculates rank for a single components in the spectrum
     * @note: Uses similarity_coefficient(const t_count n[2][2]);
     */
    virtual t_score operator () (const t_spectrum & spectrum,
                                 t_component_id comp,
                                 const t_spectrum_filter * filter=NULL) const;

    inline virtual t_score_type get_score_type () const {
        return HEURISTIC;
    }

    virtual void json_configs (t_configs & out) const;

protected:
    inline virtual std::string get_name () const {
        return "similarity";
    }

    /**
     * @brief Returns the similarity type.
     */
    virtual std::string get_similarity_type () const = 0;


    /**
     * @brief Returns the similarity coefficient for a particular component.
     * Sub-classes should override this method to implement a particular coefficient.
     */
    virtual t_score similarity_coefficient (const t_count n[2][2]) const = 0;
};
}
}

#include "similarity/jaccard.h"
#include "similarity/ochiai.h"
#include "similarity/random.h"
#include "similarity/tarantula.h"

#endif