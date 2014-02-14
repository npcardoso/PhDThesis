#ifndef __PATH_GENERATOR_H_51732833938bc903c92dbe886fb8565640d75a6a__
#define __PATH_GENERATOR_H_51732833938bc903c92dbe886fb8565640d75a6a__

#include "diagnosis/benchmark/types.h"
#include "utils/boost.h"


namespace diagnosis {
namespace benchmark {
class t_path_generator {
public:
    DEFINE_BOOST_SHARED_PTRS(t_path_generator);

    virtual t_path operator () (std::string filename) const = 0;

    virtual t_path operator () (const t_entry & entry,
                                std::string filename) const = 0;

    inline virtual ~t_path_generator () {}

    std::vector<std::string> relevant_keys;
protected:
    t_path_generator (std::string root_dir);
    t_path root_dir;
};

class t_path_single_dir : public t_path_generator {
public:
    t_path_single_dir (std::string root_dir);
    virtual t_path operator () (std::string filename) const;

    virtual t_path operator () (const t_entry & entry,
                                std::string filename) const;
};

class t_path_multi_dir : public t_path_generator {
    virtual t_path operator () (const t_entry & entry,
                                std::string filename) const;
};
}
}
#endif