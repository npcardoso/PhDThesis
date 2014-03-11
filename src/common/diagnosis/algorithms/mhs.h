#ifndef __MHS_H_957e289b8923d6c40fa69c8aa7a53a6ed1da023b__
#define __MHS_H_957e289b8923d6c40fa69c8aa7a53a6ed1da023b__

#include "diagnosis/algorithms/similarity.h"
#include "diagnosis/diagnosis_system.h"
#include "diagnosis/structs/trie.h"
#include "utils/time.h"

#include <map>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/mersenne_twister.hpp>

namespace diagnosis {
namespace algorithms {
class t_parallelization {
public:
    inline virtual bool skip (t_id rank_pos,
                              t_count depth) const {
        return false;
    }
};

class t_basic_parallelization : public t_parallelization {
public:
    t_basic_parallelization (t_count self,
                             t_count division_count,
                             t_count depth) {
        assert(self < division_count);
        assert(division_count > 0);
        this->self = self;
        this->division_count = division_count;
        this->depth = depth;
    }

protected:
    inline virtual bool skip (t_id rank_pos,
                              t_count depth) {
        if (check(depth))
            return skip(rank_pos);

        return false;
    }

    virtual bool skip (t_id rank_pos) const = 0;

    bool check (t_count depth) const {
        return depth == this->depth;
    }

    t_id self, division_count;

private:
    t_count depth;
};

class t_parallelization_random : public t_basic_parallelization {
public:
    t_parallelization_random (t_count self,
                              t_count division_count,
                              t_count depth,
                              unsigned int seed) : t_basic_parallelization(self,
                                                                           division_count,
                                                                           depth), gen(new boost::mt19937(seed)) {}

    inline virtual bool skip (t_id rank_pos) const {
        boost::random::uniform_int_distribution<t_component_id> distribution(0, division_count - 1);
        t_component_id owner = distribution(*gen);


        return owner == self;
    }

private:
    t_ptr<boost::mt19937> gen;
};

class t_parallelization_stride : public t_basic_parallelization {
public:
    using t_basic_parallelization::t_basic_parallelization;
    inline virtual bool skip (t_id rank_pos) const {
        return rank_pos % division_count == (division_count - (self + 1));
    }
};


class t_basic_cutoff {
public:

    inline virtual bool stop (t_count depth) const {
        return false;
    }

    inline virtual bool stop (const t_rank & rank,
                              t_id pos,
                              const structs::t_trie & D,
                              t_time_interval time_elapsed) const {
        return false;
    }
};

class t_cutoff : public t_basic_cutoff  {
public:
    t_cutoff () {
        max_candidates = 0;
        max_depth = 0;
        max_time = -1;
        lambda = 1;
        min_score = 0;
    }

    inline virtual bool stop (t_count depth) const {
        if (max_depth && depth > max_depth)
            return true;

        return false;
    }

    inline virtual bool stop (const t_rank & rank,
                              t_id pos,
                              const structs::t_trie & D,
                              t_time_interval time_elapsed) const {
        if (max_candidates && D.size() >= max_candidates)
            return true;

        if (max_time > 0 && time_elapsed > max_time)
            return true;

        if (rank.get_score(pos) < min_score)
            return true;

        if ((pos / (float) rank.size()) > lambda)
            return true;

        return false;
    }

public:
    t_count max_depth, max_candidates;
    t_time_interval max_time;
    float lambda;
    t_score min_score;
};


class t_mhs : public t_candidate_generator {
public:
    t_mhs (t_const_ptr<t_similarity> similarity=t_const_ptr<t_similarity> ());


    void set_cutoff (t_const_ptr<t_basic_cutoff> cutoff);
    void set_parallelization (t_const_ptr<t_parallelization> parallelization);

    virtual void operator () (const structs::t_spectra & spectra,
                              t_ret_type & D,
                              const structs::t_spectra_filter * filter=NULL) const;

    virtual void calculate (const structs::t_spectra & spectra,
                            structs::t_trie & D,
                            structs::t_spectra_filter & filter,
                            structs::t_candidate & candidate,
                            t_time_interval start_time=time_interval()) const;

    void update (const structs::t_spectra & spectra,
                 structs::t_trie & D,
                 const structs::t_trie & old_D,
                 const structs::t_spectra_filter & filter) const;


    static void combine (const structs::t_spectra & spectra,
                         structs::t_trie & D,
                         const structs::t_trie & D_first,
                         const structs::t_trie & D_second,
                         const structs::t_spectra_filter & filter_first,
                         const structs::t_spectra_filter & filter_second);

private:
    t_const_ptr<t_similarity> similarity;
    t_const_ptr<t_basic_cutoff> cutoff;
    t_const_ptr<t_parallelization> parallelization;
};
}
}

#endif