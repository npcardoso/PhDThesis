#ifndef __MEMBERSHIP_H_91121da4e84757e5b9b76386fdfa674fe072ea87__
#define __MEMBERSHIP_H_91121da4e84757e5b9b76386fdfa674fe072ea87__

#include "../types.h"
#include "../utils/boost.h"

#include <iostream>

namespace diagnosis {
namespace structs {
typedef double t_membership_value;
typedef boost::shared_ptr<t_membership_value[]> t_membership_value_ptr;

class t_membership {
public:
    t_membership ();

    t_membership (t_count points, t_count centroids);

    void set_membership (t_id point,
                         t_id centroid,
                         t_membership_value membership=0,
                         t_id offset=1);

    t_membership_value get_membership (t_id point,
                                       t_id centroid,
                                       t_id offset=1) const;

    t_id hard_cluster (t_id point, t_id offset=1) const;

    t_count get_hard_cluster_count (t_id cluster, t_id offset=1) const;

    std::ostream & print (std::ostream & out) const;

    t_count get_point_count () const;

    t_count get_centroid_count () const;

private:
    t_count num_points, num_centroids;
    t_membership_value_ptr memberships;
};

std::ostream & operator << (std::ostream & out, const t_membership & memberships);
}
}

#endif