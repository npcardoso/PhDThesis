#include "csv.h"

#include "types.h"

#include <boost/algorithm/string/trim.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>


using namespace boost;
using namespace std;


void t_csv::add_row (const t_entry & entry) {
    entries.push_back(entry);
    BOOST_FOREACH(const t_entry::value_type & e,
                  entry) {
        columns.insert(e.first);
    }
}

ostream & t_csv::write (ostream & out) const {
    // FIXME: Escape stuff
    bool first = true;


    BOOST_FOREACH(const string &key, columns) {
        if (!first)
            out << ",";
        else
            first = false;

        out << key;
    }
    out << endl;

    BOOST_FOREACH(const t_entry &e, entries) {
        first = true;
        BOOST_FOREACH(const std::string & key, columns) {
            t_entry::const_iterator it;


            it = e.find(key);

            if (!first)
                out << ",";
            else
                first = false;

            if (it != e.end())
                out << it->second;
        }
        out << endl;
    }
    return out;
}

std::istream & t_csv::read (std::istream & in) {
    typedef tokenizer<escaped_list_separator<char> > Tokenizer;
    vector<std::string> columns;
    string line;
    t_count extra = 0;

    this->entries.clear();
    this->columns.clear();

    bool first = true;

    while (getline(in, line)) {
        Tokenizer tok(line);
        vector<std::string> vec(tok.begin(), tok.end());

        // vec.assign(tok.begin(),tok.end());
        if (vec.size() <= 0)
            continue;

        // Remove extraneous spaces
        for (int i = 0; i < vec.size(); i++)
            boost::trim(vec[i]);

        if (first) {
            // Read header
            columns.assign(vec.begin(), vec.end());
            this->columns.insert(columns.begin(), columns.end());
            first = false;
        }
        else {
            // Read row

            // Insert missing keys
            for (int i = columns.size(); i < vec.size(); i++) {
                std::string new_key;

                do {
                    new_key = "X" + lexical_cast<std::string> (++extra);
                } while (this->columns.count(new_key));

                columns.push_back(new_key);
                this->columns.insert(new_key);
            }

            t_entry entry;

            for (int i = 0; i < vec.size(); i++)
                entry[columns[i]] = vec[i];

            add_row(entry);
        }
    }

    return in;
}