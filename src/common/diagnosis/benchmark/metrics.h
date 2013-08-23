#ifndef __METRICS_H_d89493d817d7ef64219d0d614f7ef3b07eb21297__
#define __METRICS_H_d89493d817d7ef64219d0d614f7ef3b07eb21297__

#include "diagnosis/diagnosis_system.h"
#include "diagnosis/structs/candidate.h"
#include "diagnosis/structs/diagnosis_report.h"
#include "utils/boost.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <vector>
#include "utils/iostream.h"

namespace diagnosis {
namespace benchmark {
class t_metric {
public:
    DEFINE_BOOST_SHARED_PTRS(t_metric);
    typedef std::map<std::string, std::string> t_ret_type;

    virtual void operator () (const structs::t_spectra & spectra,
                              const structs::t_candidate & correct,
                              const t_candidate_generator::t_ret_type & D,
                              const t_candidate_ranker::t_ret_type & probs,
                              const structs::t_diagnosis_report & dr,
                              t_ret_type & ret) = 0;
};

class t_Cd : public t_metric {
public:
    virtual void operator () (const structs::t_spectra & spectra,
                              const structs::t_candidate & correct,
                              const t_candidate_generator::t_ret_type & D,
                              const t_candidate_ranker::t_ret_type & probs,
                              const structs::t_diagnosis_report & dr,
                              t_ret_type & ret);
};

class t_wasted_effort : public t_metric {
public:
    virtual void operator () (const structs::t_spectra & spectra,
                              const structs::t_candidate & correct,
                              const t_candidate_generator::t_ret_type & D,
                              const t_candidate_ranker::t_ret_type & probs,
                              const structs::t_diagnosis_report & dr,
                              t_ret_type & ret);
};

class t_entropy : public t_metric {
public:
    virtual void operator () (const structs::t_spectra & spectra,
                              const structs::t_candidate & correct,
                              const t_candidate_generator::t_ret_type & D,
                              const t_candidate_ranker::t_ret_type & probs,
                              const structs::t_diagnosis_report & dr,
                              t_ret_type & ret);
};
}
}


#endif