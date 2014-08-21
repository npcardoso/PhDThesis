#ifndef __SIMILARITY_H_516cee6798dbd3feb110e449857eb823bcdf0260__
#define __SIMILARITY_H_516cee6798dbd3feb110e449857eb823bcdf0260__

#include "../diagnostic_system.h"
#include "../structs/spectrum.h"
#include "../types.h"
#include "../utils/boost.h"

#include <vector>

namespace diagnostic {
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


class t_similarity : public t_candidate_ranker {
public:
    // From t_candidate_ranker
    virtual void operator () (const t_spectrum & spectrum,
                              const t_trie & D,
                              t_ret_type & probs,
                              const t_spectrum_filter * filter=NULL) const;


    // Calculates rank for all components in the spectrum (does not sort nor normalize rank)
    virtual t_ptr<t_rank> operator () (const t_spectrum & spectrum,
                                       const t_spectrum_filter * filter=NULL) const;


    // Calculates rank for a single components in the spectrum
    virtual t_score operator () (const t_spectrum & spectrum,
                                 t_component_id comp,
                                 const t_spectrum_filter * filter=NULL) const;

    inline virtual t_score_type get_score_type () const {
        return HEURISTIC;
    }

    virtual void json_configs (t_configs & out) const;

protected:
    inline virtual std::string get_name() const {
        return "similarity";
    }

    virtual std::string get_similarity_type() const = 0;


    virtual t_score similarity_coefficient (const t_count n[2][2]) const = 0;
};


class t_ochiai : public t_similarity {
    public:
    inline virtual std::string get_similarity_type() const {
        return "ochiai";
    }
protected:
    virtual t_score similarity_coefficient (const t_count n[2][2]) const;

    virtual std::ostream & write (std::ostream & out) const;
};

class t_tarantula : public t_similarity {
public:
    inline virtual std::string get_similarity_type() const {
        return "tarantula";
    }
protected:
    virtual t_score similarity_coefficient (const t_count n[2][2]) const;

    virtual std::ostream & write (std::ostream & out) const;
};

class t_jaccard : public t_similarity {
public:
    inline virtual std::string get_similarity_type() const {
        return "jaccard";
    }
protected:
    virtual t_score similarity_coefficient (const t_count n[2][2]) const;

    virtual std::ostream & write (std::ostream & out) const;
};

class t_random : public t_similarity {
public:
    inline virtual std::string get_similarity_type() const {
        return "random";
    }
    // Calculates rank for a single components in the spectrum
    virtual t_score operator () (const t_spectrum & spectrum,
                                 t_component_id comp,
                                 const t_spectrum_filter * filter=NULL) const;

protected:
    inline virtual t_score similarity_coefficient (const t_count n[2][2]) const {return 0;}
};
}
}
namespace std {
ostream & operator << (ostream & out, const diagnostic::algorithms::t_rank &);
}
#endif
