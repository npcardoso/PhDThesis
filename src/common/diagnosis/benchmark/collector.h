#ifndef __COLLECTOR_H_191dcca6342c7ab0e1db44a96c7660eb47e6503f__
#define __COLLECTOR_H_191dcca6342c7ab0e1db44a96c7660eb47e6503f__

#include "diagnosis/benchmark/status.h"
#include "diagnosis/benchmark/path_generator.h"

namespace diagnosis {
namespace benchmark {
class t_collector {
public:
    DEFINE_BOOST_SHARED_PTRS(t_collector);
    t_collector (t_path_generator::t_const_ptr path_generator);

    void add_entry (const t_path & file,
                    const t_entry & entry);

    void save_file (const t_path & file,
                    const std::string & data);

    void debug (const t_status & status,
                const std::string & data);

    t_path local_path (const t_entry & entry,
                       const std::string & filename) const;
    t_path global_path (const std::string & filename) const;

protected:
    const t_path_generator & get_path_generator () const;
private:
    t_path_generator::t_const_ptr path_generator;
};
}
}
#endif