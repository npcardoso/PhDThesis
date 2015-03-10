#ifndef __JACCARD_H_99122c3415c598686a105c420ed7c005b7b4a96f__
#define __JACCARD_H_99122c3415c598686a105c420ed7c005b7b4a96f__

#include "../similarity.h"

namespace diagnostic {
namespace algorithms {
class t_jaccard : public t_similarity {
public:
    inline virtual std::string get_similarity_type () const {
        return "jaccard";
    }

protected:
    virtual t_score similarity_coefficient (const t_count n[2][2]) const;

    virtual std::ostream & write (std::ostream & out) const;
};
}
}

#endif