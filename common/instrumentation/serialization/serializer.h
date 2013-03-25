#ifndef __COMMON_INSTRUMENTATION_SERIALIZATION_SERIALIZER_H__
#define __COMMON_INSTRUMENTATION_SERIALIZATION_SERIALIZER_H__

#include <ostream>
#include "types.h"

#include "instrumentation/transaction.h"
#include "instrumentation/oracle.h"
#include "instrumentation/probe.h"

#include <boost/foreach.hpp>

class t_serializer {
public:
  typedef boost::shared_ptr<t_serializer> t_ptr;
  typedef boost::shared_ptr<const t_serializer> t_const_ptr;

  virtual std::ostream & observation(std::ostream & out,
                                     const t_oracle_observation & obs) = 0;

  virtual std::ostream & observation(std::ostream & out,
                                     const t_probe_observation & obs) = 0;

  virtual std::ostream & observation(std::ostream & out,
                                     const t_transaction_observation & obs) = 0;

  virtual std::ostream & observation_header(std::ostream & out) = 0;
  virtual std::ostream & observation_separator(std::ostream & out) = 0;
  virtual std::ostream & observation_footer(std::ostream & out) = 0;

  virtual std::ostream & observation_request_header(std::ostream & out) = 0;
  virtual std::ostream & observation_request_footer(std::ostream & out) = 0;

  virtual std::ostream & construct(std::ostream & out,
                                   const t_oracle_construct & ctr) = 0;

  virtual std::ostream & construct(std::ostream & out,
                                   const t_probe_construct & ctr) = 0;

  virtual std::ostream & construct(std::ostream & out,
                                   const t_transaction_construct & ctr) = 0;

  virtual std::ostream & construct_header(std::ostream & out) = 0;
  virtual std::ostream & construct_separator(std::ostream & out) = 0;
  virtual std::ostream & construct_footer(std::ostream & out) = 0;

  virtual std::ostream & construct_request_header(std::ostream & out) = 0;
  virtual std::ostream & construct_request_footer(std::ostream & out) = 0;

  virtual std::ostream & request_separator(std::ostream & out) = 0;

  template <class I, class C>
    static std::ostream & observation_array(std::ostream & out,
                                            const C & container,
                                            t_ptr serializer) {
      bool first = true;
      BOOST_FOREACH(I it, container) {
        if(!first)
          serializer->observation_separator(out);
        serializer->observation_header(out);
        serializer->observation(out, *it);
        serializer->observation_footer(out);
        first = false;
      }
      return out;
    }
};

#endif
