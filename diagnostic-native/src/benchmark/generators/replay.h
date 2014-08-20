#ifndef __REPLAY_H_a5bf279fece88fb8e9fd836042181b1d0d63ad35__
#define __REPLAY_H_a5bf279fece88fb8e9fd836042181b1d0d63ad35__

#include "generator.h"

#include <map>
#include <string>


namespace diagnostic {
namespace benchmark {
class t_replay : public t_spectra_generator, public std::list<std::pair<std::string, std::string> > {
public:

    virtual t_spectra * operator () (std::mt19937 & gen,
                                              t_candidate & correct_candidate);
};
}
}

#endif
