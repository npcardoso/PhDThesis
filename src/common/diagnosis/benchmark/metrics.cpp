#include "metrics.h"
namespace diagnosis {
namespace benchmark {
void t_Cd::operator () (const structs::t_spectra & spectra,
                        const structs::t_candidate & correct,
                        const t_candidate_generator::t_ret_type & D,
                        const t_candidate_ranker::t_ret_type & probs,
                        const structs::t_diagnosis_report & dr,
                        t_metric::t_ret_type & ret) {
    t_candidate_ranker::t_ret_type::value_type current = NAN;
    t_count total_elements = 0, elements = 0;
    bool quit = false;


    for (t_id i = 1; i <= dr.size(); i++) {
        if (dr.get_probability(i) == current)
            elements++;
        else {
            if (quit)
                break;

            current = dr.get_probability(i);
            elements = 1;
        }

        total_elements++;

        if (correct == dr.get_candidate(i))
            quit = true;
    }

    if (!quit)
        // Candidate not even considered
        ret["Cd"] = "NAN";
    else {
        assert(((total_elements - 1) - ((elements - 1) / 2.0)) >= 0);
        ret["Cd"] = boost::lexical_cast<std::string> ((total_elements - 1) - ((elements - 1) / 2.0));
    }
}
}
}