#ifndef __HEURISTIC_H__
#define __HEURISTIC_H__

#include "diagnosis/spectra.h"
#include "diagnosis/spectra_filter.h"

#include <vector>

namespace diagnosis {

class t_heuristic_filter {
public:
  virtual void operator()(const t_spectra & spectra, 
                          t_rank_element * ret,
                          const t_spectra_filter * filter = NULL) const = 0;
  virtual std::ostream & print(std::ostream & out) const;
};

std::ostream & operator << (std::ostream & out, const t_heuristic_filter & filter);

class t_heuristic {
  typedef boost::shared_ptr<const t_heuristic_filter > t_filter_ptr;
  typedef std::vector<t_filter_ptr> t_filters;
  t_filters filters;

public:

  void push(const t_filter_ptr & filter);

  void push(const t_heuristic_filter * filter);

  void operator()(const t_spectra & spectra, 
                  t_rank_element * ret,
                  const t_spectra_filter * filter = NULL) const;

  virtual std::ostream & print(std::ostream & out) const;
};

}

std::ostream & operator << (std::ostream & out, const diagnosis::t_heuristic & heuristic);

#endif
