#ifndef __HEURISTIC_H__
#define __HEURISTIC_H__

#include "../structs/spectra.h"
#include "../structs/spectra_filter.h"

#include <boost/shared_ptr.hpp>

#include <vector>

template <class T_ACTIVITY>
  class t_heuristic_filter {
  public:
    virtual void operator()(const t_spectra <T_ACTIVITY> & spectra, 
                            t_rank_element * ret,
                            const t_spectra_filter * filter = NULL) const = 0;
    virtual std::ostream & print(std::ostream & out) const {
      return out << "Not Implemented";
    }
  };

template <class T_ACTIVITY>
  inline std::ostream & operator << (std::ostream & out, const t_heuristic_filter <T_ACTIVITY> & filter){
    return filter.print(out);
  }

template <class T_ACTIVITY>
  class t_heuristic {
    typedef boost::shared_ptr<const t_heuristic_filter <T_ACTIVITY> > t_filter_ptr;
    typedef std::vector<t_filter_ptr> t_filters;
    t_filters filters;

  public:

    void push(const t_filter_ptr & filter) {
      filters.push_back(filter);
    }

    void push(const t_heuristic_filter <T_ACTIVITY> * filter) {
      filters.push_back(t_filter_ptr(filter));
    }

    void operator()(const t_spectra <T_ACTIVITY> & spectra, 
                    t_rank_element * ret,
                    const t_spectra_filter * filter = NULL) const {
      assert(ret != NULL);

      if(!spectra.get_transaction_count(filter))
        return;

      if(filters.size()) {
        typename t_filters::const_iterator it = filters.begin();
        ret[0] = t_rank_element(0, 0);

        while(it != filters.end())
          (**(it++))(spectra, ret, filter);
      }
      else {
        t_spectra_iterator it(spectra.get_component_count(),
                              spectra.get_transaction_count(),
                              filter);
        t_id i = 0;
        while (it.next_component())
          ret[i++] = t_rank_element(it.get_component(), 1);
      }
    }

    virtual std::ostream & print(std::ostream & out) const {
      typename t_filters::const_iterator it = filters.begin();
      out << "t_heuristic(";
      if(it != filters.end())
        out << **(it++);

      while(it != filters.end())
        out << ", " << **(it++);
      return out << ")";
    }
  };

template <class T_ACTIVITY>
  inline std::ostream & operator << (std::ostream & out, const t_heuristic <T_ACTIVITY> & heuristic){
    return heuristic.print(out);
  }

#endif
