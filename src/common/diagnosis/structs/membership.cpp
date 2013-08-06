#include "diagnosis/structs/membership.h"

namespace diagnosis {
namespace structs {
t_membership::t_membership () {}

t_membership::t_membership (t_count points, t_count centroids) : num_points(points), num_centroids(centroids) {
    this->memberships =
        t_membership_value_ptr(new t_membership_value[num_points * num_centroids]);
}

void t_membership::set_membership (t_id point,
                                   t_id centroid,
                                   t_membership_value membership,
                                   t_id offset) {
    point -= offset;
    centroid -= offset;
    assert(point >= 0 && point < num_points);
    assert(centroid >= 0 && centroid < num_centroids);

    memberships[point * num_centroids + centroid] = membership;
}

t_membership_value t_membership::get_membership (t_id point,
                                                 t_id centroid,
                                                 t_id offset) const {
    point -= offset;
    centroid -= offset;
    assert(point >= 0 && point < num_points);
    assert(centroid >= 0 && centroid < num_centroids);

    return memberships[point * num_centroids + centroid];
}

t_id t_membership::hard_cluster (t_id point, t_id offset) const {
    point -= offset;
    assert(point >= 0 && point < num_points);
    assert(num_centroids >= 1);

    t_id point_offset = point * num_centroids;
    t_membership_value current, max = memberships[point_offset];
    t_id centroid = 0;

    for (t_id c = 1; c < num_centroids; c++) {
        current = memberships[point_offset + c];

        if (current > max) {
            max = current;
            centroid = c;
        }
    }

    return centroid + offset;
}

t_count t_membership::get_hard_cluster_count (t_id cluster, t_id offset) const {
    cluster -= offset;
    assert(cluster >= 0 && cluster < num_centroids);

    t_count count = 0;

    for (t_id p = 0; p < num_points; p++)
        if (hard_cluster(p, 0) == cluster)
            count++;

    return count;
}

t_count t_membership::get_point_count () const {
    return num_points;
}

t_count t_membership::get_centroid_count () const {
    return num_centroids;
}

std::ostream & t_membership::print (std::ostream & out) const {
    for (t_id p = 1; p <= num_points; p++) {
        out << p << '\t';

        for (t_id c = 1; c <= num_centroids; c++) {
            out << get_membership(p, c) << '\t';
        }

        out << std::endl;
    }

    return out;
}

std::ostream & operator << (std::ostream & out, const t_membership & memberships) {
    return memberships.print(out);
}
}
}