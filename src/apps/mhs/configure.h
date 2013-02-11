#ifndef __CONFIGURE_MHS_H__
#define __CONFIGURE_MHS_H__

#include "../../algorithms/mhs.h"
#include "../../common/opt.h"
#include "../../common/types.h"
#include "../../heuristic/parallelization.h"
#include "../../heuristic/similarity.h"
#include "../../heuristic/sort.h"

template <class T_ACTIVITY>
class t_mhs_options: public t_options {
public:
  t_count mpi_level;
  t_count mpi_stride;


  t_mhs<T_ACTIVITY> mhs;

  static t_heuristic<T_ACTIVITY> default_heuristic() {
    t_heuristic<T_ACTIVITY> heuristic;
    heuristic.push(new t_filter_ochiai<t_count>());
    heuristic.push(new t_filter_sort<t_count>());
    return heuristic;
  }

  inline t_mhs_options(std::string app_name):t_options(app_name, true, true), mhs(default_heuristic()) {
    mpi_level = 0;
    mpi_stride = 0;
    
    add(t_opt('l', "mpi-level", true, false, "Sets the forking level"));
    add(t_opt('s', "mpi-stride", true, false, "Sets the stride factor"));
    add(t_opt('t', "time", true, false, "Sets maximum computation time"));
    add(t_opt('d', "candidates", true, false, "Sets maximum number of candidates"));
    add(t_opt('c', "cardinality", true, false, "Sets maximum candidate cardinality"));
  }

  virtual bool short_opt(int c, char * param) {
    switch (c) {
    case 't':
      return verb_strtof(optarg, mhs.max_time, true);
    case 'd':
      return verb_strtoi(optarg, mhs.max_candidates, true);
    case 'c':
      return verb_strtoi(optarg, mhs.max_candidate_size, true);
    case 'l':
      return verb_strtoi(optarg, mpi_level, true);
    case 's':
      return verb_strtoi(optarg, mpi_stride, true);
    default:
      return t_options::short_opt(c, param);
    }
    return false;
  }

  virtual std::ostream & print(std::ostream & out) const {
    t_options::print(out);
    out << ", MPI_level: " << mpi_level;
    out << ", MPI_stride: " << mpi_stride;
    out << ", Cardinality: " <<mhs.max_candidate_size;
    out << ", Candidates: " << mhs.max_candidates;
    out << ", Time: " << mhs.max_time;
    return out;
  }
};
#endif
