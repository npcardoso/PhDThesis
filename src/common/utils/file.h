#ifndef __FILE_H_b9537bb091f448b212cc12402a957a55fc88e546__
#define __FILE_H_b9537bb091f448b212cc12402a957a55fc88e546__


#include "utils/boost.h"
#include <boost/thread.hpp>
#include <fstream>

class t_file {
public:
    DEFINE_BOOST_SHARED_PTRS(t_file);
    virtual void flush () = 0;
    const t_path & get_path () const;
    bool operator < (const t_file & other) const;

protected:
    t_file (const t_path & path);

    bool open_file (std::ifstream & stream);
    bool open_file (std::ofstream & stream);

    boost::mutex mutex;
private:
    t_path path;
};


#endif