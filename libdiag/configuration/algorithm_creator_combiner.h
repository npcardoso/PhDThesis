#ifndef __ALGORITHM_CREATOR_COMBINER_H_969acdf20c1caaf5385f29ba68102e80bd417d74__
#define __ALGORITHM_CREATOR_COMBINER_H_969acdf20c1caaf5385f29ba68102e80bd417d74__

#include "algorithm_creator.h"

namespace diagnostic {
namespace configuration {
class t_algorithm_creator_combiner: public t_algorithm_creator {
    public:
    virtual t_const_ptr<t_candidate_generator> generator(const t_algorithm & a) const;
    virtual t_const_ptr<t_candidate_ranker> ranker(const t_algorithm & a) const;

    void add(t_const_ptr<t_algorithm_creator> c);

    private:
    typedef std::list<t_const_ptr<t_algorithm_creator> > t_creators;
    t_creators creators;
};
}
}
#endif
