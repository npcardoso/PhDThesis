#ifndef __PARALLELIZATION_H_5932959307b5185825a79718965aed2491de5b43__
#define __PARALLELIZATION_H_5932959307b5185825a79718965aed2491de5b43__

#include "types.h"
#include "structs/candidate.h"
#include "utils/boost.h"

#include <random>

namespace diagnosis {
namespace algorithms {
class t_parallelization {
public:
    virtual bool skip (t_id rank_pos,
                       const structs::t_candidate & candidate) const;
};

class t_basic_parallelization : public t_parallelization {
public:
    t_basic_parallelization (t_count self,
                             t_count division_count,
                             t_count depth);
    virtual bool skip (t_id rank_pos,
                       const structs::t_candidate & candidate) const;
protected:
    virtual bool skip (t_id rank_pos) const = 0;

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
    t_ptr<std::mt19937> gen;
};

class t_parallelization_stride : public t_basic_parallelization {
public:
    using t_basic_parallelization::t_basic_parallelization;
    virtual bool skip (t_id rank_pos) const;
};


class t_parallelization_factory {
public:
    t_parallelization_factory (t_count depth);
    virtual t_parallelization * operator () (t_count self,
                                             t_count division_count) const = 0;
protected:
    t_count depth;
};

class t_parallelization_factory_stride : public t_parallelization_factory {
public:
    using t_parallelization_factory::t_parallelization_factory;
    virtual t_parallelization * operator () (t_count self,
                                             t_count division_count) const;
};

class t_parallelization_factory_random : public t_parallelization_factory {
public:
    t_parallelization_factory_random (t_count depth,
                                      unsigned int seed=0);

    virtual t_parallelization * operator () (t_count self,
                                             t_count division_count) const;

private:
    unsigned int seed;
};
}
}

#endif