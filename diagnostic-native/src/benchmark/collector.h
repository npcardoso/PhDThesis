#ifndef __COLLECTOR_H_191dcca6342c7ab0e1db44a96c7660eb47e6503f__
#define __COLLECTOR_H_191dcca6342c7ab0e1db44a96c7660eb47e6503f__

#include "path_generator.h"
#include "status.h"
#include "../utils/boost.h"

#include <boost/thread.hpp>
#include <fstream>

namespace diagnostic {
namespace benchmark {
typedef std::map<std::string, std::string> t_entry;

class t_file {
public:
    inline virtual ~t_file () {}

    virtual void flush () = 0;
    const t_path & get_path () const;
    bool exists () const;
    bool operator < (const t_file & other) const;

protected:
    t_file (const t_path & path);

    bool open (std::ifstream & stream) const;
    bool open (std::ofstream & stream, bool append=false) const;

    boost::mutex mutex;
private:
    t_path path;
};


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
