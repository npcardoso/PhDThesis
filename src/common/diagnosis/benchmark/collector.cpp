#include "collector.h"

#include "utils/csv.h"
#include "utils/iostream.h"

#include <boost/foreach.hpp>
#include <sstream>
#include <iostream>
#include <fstream>

using namespace std;
using namespace boost::filesystem;


namespace diagnosis {
namespace benchmark {
class t_csv_file : public t_file {
public:
    t_csv_file (const t_path & path) : t_file(path) {
        boost::mutex::scoped_lock lock(mutex);
        ifstream in;


        open_file(in);
        csv.read(in);
        in.close();

        need_flush = false;
    }

    virtual void flush () {
        boost::mutex::scoped_lock lock(mutex);


        if (need_flush) {
            ofstream out;


            open_file(out);
            csv.write(out);
            out.close();
            std::cerr << "Flushing csv file: " << get_path() << std::endl;
            need_flush = false;
        }
    }

    void add_row (const t_entry & entry) {
        boost::mutex::scoped_lock lock(mutex);


        csv.add_row(entry);
        need_flush = true;
    }

private:
    bool need_flush;
    t_csv csv;
};

class t_normal_file : public t_file {
public:
    t_normal_file (const t_path & path) : t_file(path) {}

    void save (const std::string & data) {
        boost::mutex::scoped_lock lock(mutex);
        ofstream out;


        open_file(out);
        out << data;
        out.close();
    }

    void flush () {}
};


t_collector::t_collector (t_const_ptr<t_path_generator> path_generator) : path_generator(path_generator) {}

void t_collector::add_entry (const t_path & path,
                             const t_entry & entry) {
    t_csv_file * file = get_file<t_csv_file> (path.string());


    // TODO: Handle cases where file == NULL
    if (file)
        file->add_row(entry);
}

void t_collector::save_file (const t_path & path,
                             const std::string & data) {
    t_normal_file * file = get_file<t_normal_file> (path.string());


    // TODO: Handle cases where file == NULL
    if (file)
        file->save(data);
}

void t_collector::debug (const t_status & status,
                         const std::string & data) {
    boost::mutex::scoped_lock lock(mutex);
    t_entry entry;


    status.prepare_entry(entry);
    std::stringstream ss(data);

    while (ss.good()) {
        std::string line;
        getline(ss, line);
        std::cerr << "Debug:" << entry << ":" << line << std::endl;
    }
}

t_path t_collector::local_path (const t_entry & entry,
                                const std::string & filename) const {
    return get_path_generator() (entry,
                                 filename);
}

t_path t_collector::global_path (const std::string & filename) const {
    return get_path_generator() (filename);
}

void t_collector::flush_all () {
    boost::mutex::scoped_lock lock(mutex);


    BOOST_FOREACH(t_files::value_type file, files) {
        file->flush();
    }
    // files.clear();
    // paths.clear();
}

t_collector::~t_collector () {
    flush_all();
}

const t_path_generator & t_collector::get_path_generator () const {
    return *path_generator;
}
}
}