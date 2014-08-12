#ifndef __SIMILARITY_CREATOR_H_a94eca3c2be68e5d13c1a8242032673ffcb34532__
#define __SIMILARITY_CREATOR_H_a94eca3c2be68e5d13c1a8242032673ffcb34532__

#include "algorithm_creator.h"

namespace diagnostic {
namespace configuration {

class t_similarity_creator : public t_algorithm_creator {
    public:
    virtual t_const_ptr<t_candidate_ranker> ranker(const t_algorithm & a) const;
};
}
}
#endif
