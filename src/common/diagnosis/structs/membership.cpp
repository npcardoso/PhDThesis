#include "diagnosis/structs/membership.h"

namespace diagnosis {
namespace structs {

  void t_membership::set_membership(t_id point, 
                                    t_id centroid, 
                                    t_membership_value membership, 
                                    t_id offset) {
    point -= offset;
    centroid -= offset;
    assert(point >= 0 && point < num_points);
    assert(centroid >= 0 && centroid < num_centroids);
    
    memberships[point * num_centroids + centroid] = membership;
  }
  
  t_membership_value t_membership::get_membership(t_id point, 
                                                  t_id centroid, 
                                                  t_id offset) const {
    point -= offset;
    centroid -= offset;
    assert(point >= 0 && point < num_points);
    assert(centroid >= 0 && centroid < num_centroids);
    
    return memberships[point * num_centroids + centroid];
  }
  
  t_count t_membership::get_num_points() const {
    return num_points;
  }
  
  t_count t_membership::get_num_centroids() const {
    return num_centroids;
  }
  
  std::ostream & t_membership::print(std::ostream & out) const {
    for(t_id p = 1; p <= num_points; p++) {
      out << p << '\t';
      for(t_id c = 1; c <= num_centroids; c++) {
        out << get_membership(p,c) << '\t';
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
