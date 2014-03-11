#ifndef __PARALLELIZATION_H_5932959307b5185825a79718965aed2491de5b43__
#define __PARALLELIZATION_H_5932959307b5185825a79718965aed2491de5b43__

#include "types.h"
#include "utils/boost.h"

#include <boost/random/mersenne_twister.hpp>

namespace diagnosis {
namespace algorithms {
class t_parallelization {
public:
    virtual bool skip (t_id rank_pos,
                       t_count depth) const;
};

class t_basic_parallelization : public t_parallelization {
public:
    t_basic_parallelization (t_count self,
                             t_count division_count,
                             t_count depth);
protected:
    virtual bool skip (t_id rank_pos,
                       t_count depth);
    virtual bool skip (t_id rank_pos) const = 0;
    bool check (t_count depth) const;


    t_id self, division_count;
private:
    t_count depth;
};

class t_parallelization_random : public t_basic_parallelization {
public:
    t_parallelization_random (t_count self,
                              t_count division_count,
                              t_count depth,
                              unsigned int seed);

    virtual bool skip (t_id rank_pos) const;
private:
    t_ptr<boost::mt19937> gen;
};

class t_parallelization_stride : public t_basic_parallelization {
public:
    using t_basic_parallelization::t_basic_parallelization;
    virtual bool skip (t_id rank_pos) const;
};
}
}

#endif