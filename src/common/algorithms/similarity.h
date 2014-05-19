#ifndef __SIMILARITY_H_516cee6798dbd3feb110e449857eb823bcdf0260__
#define __SIMILARITY_H_516cee6798dbd3feb110e449857eb823bcdf0260__

#include "types.h"
#include "structs/spectra.h"
#include "utils/boost.h"

#include <vector>

namespace diagnosis {
namespace algorithms {
class t_rank {
public:

    t_rank (t_count size);

    void set (t_id i,
              t_score s,
              t_component_id c);

    void sort ();
    void normalize ();

    t_component_id get_component (t_id i) const;
    t_score get_score (t_id i) const;
    t_count size () const;


private:
    typedef std::pair<t_score, t_component_id> t_rank_element;
    std::vector<t_rank_element> rank;
};


class t_similarity {
public:
    // Calculates rank for all components in the spectra (does not sort nor normalize rank)
    virtual t_ptr<t_rank> operator () (const structs::t_spectra & spectra,
                                       const structs::t_spectra_filter * filter=NULL) const;


    // Calculates rank for a single components in the spectra
    virtual t_score operator () (const structs::t_spectra & spectra,
                                 t_component_id comp,
                                 const structs::t_spectra_filter * filter=NULL) const;

protected:
    virtual t_score similarity_coefficient (const t_count n[2][2]) const = 0;
};


class t_ochiai : public t_similarity {
protected:
    virtual t_score similarity_coefficient (const t_count n[2][2]) const;

    virtual std::ostream & print (std::ostream & out) const;
};

class t_tarantula : public t_similarity {
protected:
    virtual t_score similarity_coefficient (const t_count n[2][2]) const;

    virtual std::ostream & print (std::ostream & out) const;
};

class t_jaccard : public t_similarity {
protected:
    virtual t_score similarity_coefficient (const t_count n[2][2]) const;

    virtual std::ostream & print (std::ostream & out) const;
};

class t_random : public t_similarity {
    // Calculates rank for a single components in the spectra
    virtual t_score operator () (const structs::t_spectra & spectra,
                                 t_component_id comp,
                                 const structs::t_spectra_filter * filter=NULL) const;

protected:
    inline virtual t_score similarity_coefficient (const t_count n[2][2]) const {return 0;}
};
}
}
namespace std {
ostream & operator << (ostream & out, const diagnosis::algorithms::t_rank &);
}
#endif