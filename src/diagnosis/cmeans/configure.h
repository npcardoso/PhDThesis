#ifndef __CONFIGURE_H__
#define __CONFIGURE_H__

#include "opt.h"
#include "types.h"
#include "diagnosis/algorithms/cmeans.h"

using namespace diagnosis;

class t_cmeans_options : public t_options {
public:
    algorithms::t_cmeans_configs configs;

    inline t_cmeans_options (std::string app_name) : t_options(app_name, true, true) {
        add(t_opt('m', "fuzzifier", true, false, "Sets the level of cluster fuzziness"));
        add(t_opt('e', "epsilon", true, false, "Sets the sensitivity threshold"));
        add(t_opt('n', "iterations", true, false, "Sets the maximum number of iterations"));
        add(t_opt('f', "fixed-centroids", true, false, "Sets the number of fixed centroids"));
        add(t_opt('d', "distance", true, false, "Sets the distance function"));
    }

private:
    bool handle_distance (std::string distance) {
        if (distance == "euclidean")
            configs.dist_function =
                t_distance_function::t_ptr(new diagnosis::t_euclidean_distance());
        else if (distance == "manhattan")
            configs.dist_function =
                t_distance_function::t_ptr(new diagnosis::t_manhattan_distance());

        return false;
    }

    virtual bool short_opt (int c, char * param) {
        switch (c) {
        case 'm':
            return verb_strtof(optarg, configs.m, true);

        case 'e':
            return verb_strtof(optarg, configs.epsilon, true);

        case 'n':
            return verb_strtoi(optarg, configs.num_iterations, true);

        case 'f':
            return verb_strtoi(optarg, configs.locked_centroids, true);

        case 'd':
            return handle_distance(optarg);

        default:
            return t_options::short_opt(c, param);
        }

        return false;
    }
};

#endif