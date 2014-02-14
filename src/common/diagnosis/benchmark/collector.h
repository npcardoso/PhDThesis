#ifndef __COLLECTOR_H_191dcca6342c7ab0e1db44a96c7660eb47e6503f__
#define __COLLECTOR_H_191dcca6342c7ab0e1db44a96c7660eb47e6503f__

#include "diagnosis/benchmark/status.h"
#include "diagnosis/benchmark/path_generator.h"
#include "utils/file.h"

#include <boost/thread.hpp>

namespace diagnosis {
namespace benchmark {
class t_collector {
public:
    t_collector (t_const_ptr<t_path_generator> path_generator);

    void add_entry (const t_path & path,
                    const t_entry & entry);

    void save_file (const t_path & path,
                    const std::string & data);

    void debug (const t_status & status,
                const std::string & data);

    t_path local_path (const t_entry & entry,
                       const std::string & filename) const;
    t_path global_path (const std::string & filename) const;

    void flush_all ();


    ~t_collector ();
private:
    const t_path_generator & get_path_generator () const;


    typedef std::map<std::string, t_id> t_paths;
    t_paths paths;

    typedef std::vector < t_ptr < t_file >> t_files;
    t_files files;

    boost::mutex mutex;

    t_const_ptr<t_path_generator> path_generator;

    template <class T>
    T * get_file (const std::string & path) {
        boost::mutex::scoped_lock lock(mutex);

        t_paths::iterator it = paths.find(path);


        if (it == paths.end()) {
            T * file = new T(path);
            files.push_back(t_ptr<t_file> (file));
            paths[path] = files.size() - 1;
            return file;
        }

        return dynamic_cast<T *> (files[it->second].get());
    }
};
}
}
#endif