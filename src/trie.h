#include "types.h"

#include <map>
#include <boost/lexical_cast.hpp>

class t_trie {
  typedef std::map<t_component_id, t_trie> t_children;

  t_children children;
  bool exists;
  
  bool add(const t_candidate & candidate, 
           t_candidate::const_iterator component,
           bool composites);

  bool purge_composites(const t_candidate & candidate,
                        t_candidate::const_iterator component);
  
  bool is_composite(const t_candidate & candidate,
                    t_candidate::const_iterator component) const;

public:
  inline t_trie() {
    exists = false;
  }

  inline void add(const t_candidate & candidate, bool composites=false) {
    if(!composites) {
      if(is_composite(candidate))
        return;
      else
        purge_composites(candidate, candidate.begin());
    }
    add(candidate, candidate.begin(), composites);  
  }

  inline bool is_composite(const t_candidate & candidate) const {
    return is_composite(candidate, candidate.begin());  
  }

  inline virtual std::ostream & print(std::ostream & out, std::string prefix = "") const {
    if(exists)
      out << prefix << std::endl;
    t_children::const_iterator it = children.begin();

    while(it != children.end()) {
      it->second.print(out, prefix + "," + boost::lexical_cast<std::string>(it->first));
      it++;
    }
    return out;  
  }

};
