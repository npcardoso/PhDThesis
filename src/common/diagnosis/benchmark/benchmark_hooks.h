#ifndef __BENCHMARK_HOOKS_H__0734b6b340e40970218b6c20df0194edd574e1c1__
#define __BENCHMARK_HOOKS_H__0734b6b340e40970218b6c20df0194edd574e1c1__

#include "diagnosis/structs/candidate.h"
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <fstream>

namespace diagnosis {
namespace benchmark {
template <class S>
class t_benchmark_hooks {
public:
    typedef S t_spectra_type;
    virtual void init (const t_spectra_type & spectra,
                       const structs::t_candidate & correct) = 0;

    virtual void cleanup () = 0;

    virtual void pre_gen (t_id generator_id) = 0;
    virtual void post_gen (t_candidate_generator::t_ret_type & D) = 0;

    virtual void pre_rank (t_id ranker_id) = 0;
    virtual void post_rank (const t_candidate_ranker::t_ret_type & probs) = 0;
};

template <class S>
class t_verbose_hook : public t_benchmark_hooks<S> {
public:
    typedef S t_spectra_type;
    virtual void init (const t_spectra_type & spectra,
                       const structs::t_candidate & correct) {
        std::cerr << "Init Hook" << std::endl << spectra << std::endl << correct << std::endl;
    }

    virtual void cleanup () {
        std::cerr << "Cleanup Hook" << std::endl;
    }

    virtual void pre_gen (t_id generator_id) {
        std::cerr << "Pre_gen Hook, generator_id: " << generator_id << std::endl;
    }

    virtual void post_gen (t_candidate_generator::t_ret_type & D) {
        std::cerr << "Post_gen Hook, |D|: " << D.size() << std::endl;
    }

    virtual void pre_rank (t_id ranker_id) {
        std::cerr << "Pre_rank Hook, ranker_id: " << ranker_id << std::endl;
    }

    virtual void post_rank (const t_candidate_ranker::t_ret_type & probs) {
        std::cerr << "Post_rank Hook" << std::endl;
    }
};

template <class S>
class t_save_hook : public t_benchmark_hooks<S> {
public:
    typedef S t_spectra_type;
    typedef boost::filesystem::path t_path;

    t_save_hook (std::string d) : root_dir(d) {
        boost::filesystem::create_directories(root_dir);


        iterations = read_iterations();
        std::cerr << "Starting at iteration " << iterations << std::endl;
        generator_id = 0;
        ranker_id = 0;
    }

    virtual void init (const t_spectra_type & spectra,
                       const structs::t_candidate & correct) {
        assert(this->generator_id == 0);
        assert(this->ranker_id == 0);
        iterations++;

        t_path base_file(root_dir);
        base_file /= boost::lexical_cast<std::string> (iterations);
        base_file += ".spectra";

        std::ofstream f_spectra(base_file.c_str());
        f_spectra.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        f_spectra << spectra;
        f_spectra.close();
    }

    virtual void cleanup () {
        assert(this->ranker_id == 0);
        generator_id = 0;
    }

    virtual void pre_gen (t_id generator_id) {
        this->generator_id = generator_id;
    }

    virtual void post_gen (t_candidate_generator::t_ret_type & D) {
        assert(this->generator_id != 0);

        t_path base_file(root_dir);
        base_file /= boost::lexical_cast<std::string> (iterations);
        base_file += ".";
        base_file += boost::lexical_cast<std::string> (generator_id);
        base_file += ".D";

        std::ofstream f_D(base_file.c_str());
        f_D.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        f_D << D;
        f_D.close();

        // std::ofstream f_stats((t_path(base_file) += ".stats").c_str());
        // f_stats.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        // f_stats.close();
    }

    virtual void pre_rank (t_id ranker_id) {
        assert(this->generator_id != 0);
        assert(this->ranker_id == 0);

        this->ranker_id = ranker_id;
    }

    virtual void post_rank (const t_candidate_ranker::t_ret_type & probs) {
        assert(this->generator_id != 0);
        assert(this->ranker_id != 0);

        t_path base_file(root_dir);
        base_file /= boost::lexical_cast<std::string> (iterations);
        base_file += ".";
        base_file += boost::lexical_cast<std::string> (generator_id);
        base_file += ".";
        base_file += boost::lexical_cast<std::string> (ranker_id);
        base_file += ".Probs";

        std::ofstream f(base_file.c_str());
        f << probs;
        f.close();
        ranker_id = 0;
    }

private:
    t_count write_iterations () const {
        t_path f_iterations(root_dir);


        f_iterations /= "num_iteration.txt";

        std::ifstream f(f_iterations.c_str());
        t_count iterations;

        f >> iterations;

        if (f.good())
            return iterations;

        return 0;
    }

    t_count read_iterations () const {
        t_path f_iterations(root_dir);


        f_iterations /= "num_iteration.txt";

        std::ifstream f(f_iterations.c_str());
        t_count iterations;

        f >> iterations;

        if (f.good())
            return iterations;

        return 0;
    }

private:
    t_path root_dir;
    t_count iterations;
    t_id generator_id;
    t_id ranker_id;
};
}
}

#endif