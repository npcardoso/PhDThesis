#include "verbose_hook.h"

namespace diagnosis {
namespace benchmark {
void t_verbose_hook::_init (const structs::t_spectra & spectra,
                            const structs::t_candidate & correct) {
    std::cerr << "Init Hook" << std::endl << spectra << std::endl << correct << std::endl;
}

void t_verbose_hook::_cleanup () {
    std::cerr << "Cleanup Hook" << std::endl;
}

void t_verbose_hook::_pre_gen () {
    std::cerr << "Pre_gen Hook, generator_id: " << get_generator_id() << std::endl;
}

void t_verbose_hook::_post_gen (t_candidate_generator::t_ret_type & D,
                                t_time_interval duration) {
    std::cerr << "Post_gen Hook, |D|: " << D.size() << ". Took " << duration << "ms" << std::endl;
}

void t_verbose_hook::_pre_rank () {
    std::cerr << "Pre_rank Hook, generator_id: " << get_generator_id() << " ranker_id: " << get_ranker_id() << std::endl;
}

void t_verbose_hook::_post_rank (const t_candidate_ranker::t_ret_type & probs,
                                 t_time_interval duration) {
    std::cerr << "Post_rank Hook. Took " << duration << "ms" << std::endl;
}
}
}