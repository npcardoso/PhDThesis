#include "collector.h"

#include "../utils/iostream.h"

#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <random>
#include <sstream>
#include <iostream>
#include <fstream>

using namespace std;


namespace diagnosis {
namespace benchmark {
t_file::t_file (const t_path & path) {
    this->path = path;
}

const t_path & t_file::get_path () const {
    return path;
}

bool t_file::exists () const {
    return boost::filesystem::exists(path);
}

bool t_file::open (ifstream & stream) const {
    if (!exists())
        return false;

    stream.open(path.string().c_str());
    return true;
}

bool t_file::open (ofstream & stream, bool append) const {
    if (!exists())
        create_directories(path.parent_path());

    stream.open(path.string().c_str(), append ? std::ios_base::app : std::ios_base::ate);
    return true;
}

bool t_file::operator < (const t_file & other) const {
    return path < other.path;
}

class t_csv_file : public t_file {
public:
    t_csv_file (const t_path & path) : t_file(path), gen(time_interval()) {}

    virtual void flush () {
        boost::mutex::scoped_lock lock(mutex);
        ofstream out;
        bool put_header = !exists();


        open(out, true);

        if (put_header)
            out << "id, variable, value" << std::endl;

        BOOST_FOREACH(const auto & e, entries) {
            t_id id = std::uniform_int_distribution<t_id> () (gen);


            BOOST_FOREACH(const auto & v, e) {
                out << id << ", \"" << v.first << "\", " <<
                    '"' << v.second << '"' << std::endl;
            }
        }
        entries.clear();
        out.close();
    }

    void add_entry (const t_entry & entry) {
        boost::mutex::scoped_lock lock(mutex);


        entries.push_back(entry);
    }

private:
    std::list<t_entry> entries;
    std::mt19937 gen;
};

class t_normal_file : public t_file {
public:
    t_normal_file (const t_path & path) : t_file(path) {}

    void save (const std::string & data) {
        boost::mutex::scoped_lock lock(mutex);
        ofstream out;


        open(out);
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
        file->add_entry(entry);
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


    BOOST_FOREACH(t_files::value_type & file, files) {
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