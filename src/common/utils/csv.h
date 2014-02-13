#ifndef __CSV_H_7d192b1b4a127af61f33724db461a3533963cf89__
#define __CSV_H_7d192b1b4a127af61f33724db461a3533963cf89__

#include <list>
#include <map>
#include <set>
#include <string>
#include <iostream>

typedef std::map<std::string, std::string> t_entry;

class t_csv {
public:
    void add_row (const t_entry & entry);
    std::ostream & write (std::ostream & out) const;
    std::istream & read (std::istream & in);
private:
    std::set<std::string> columns;
    std::list<t_entry> entries;
};

#endif