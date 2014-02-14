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
    }

    virtual void flush () {
        boost::mutex::scoped_lock lock(mutex);
        ofstream out;


        open_file(out);
        csv.write(out);
        out.close();
        std::cerr << "Flushing csv file: " << get_path() << std::endl;
    }

    void add_row (const t_entry & entry) {
        boost::mutex::scoped_lock lock(mutex);


        csv.add_row(entry);
    }

private:
    t_csv csv;
};


t_collector::t_collector (t_path_generator::t_const_ptr path_generator) : path_generator(path_generator) {}

void t_collector::add_entry (const t_path & path,
                             const t_entry & entry) {
    t_csv_file * file = get_file<t_csv_file> (path.string());


    // TODO: Handle cases where file == NULL
    if (file)
        file->add_row(entry);
}

void t_collector::save_file (const t_path & file,
                             const std::string & data) {
    return;

    std::cerr << "Saving File \"";
    std::cerr << file << "\":" << std::endl;
    std::cerr << data << std::endl;
}

void t_collector::debug (const t_status & status,
                         const std::string & data) {
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
    files.clear();
    paths.clear();
}

t_collector::~t_collector () {
    flush_all();
}

const t_path_generator & t_collector::get_path_generator () const {
    return *path_generator;
}
}
}