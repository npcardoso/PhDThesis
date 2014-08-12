#include <crowbar/diagnostic/diag.h>

#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

using boost::property_tree::ptree;
using boost::property_tree::ptree_bad_path;
using boost::property_tree::read_json;
using boost::property_tree::write_json;
using namespace std;
using namespace diagnostic;
using namespace diagnostic::algorithms;


void print(ptree pt, string prefix=""){
    BOOST_FOREACH(ptree::value_type & e, pt) {
        cout << prefix +"> " + e.first << ":"  << e.second.get_value<string>() << std::endl;
        print(e.second, prefix+" ");
    }
}

class t_diagnostic_system {
public:
    typedef std::pair<t_id, t_id> t_connection;
    typedef std::vector<t_connection> t_connections;

    void add_generator (const t_const_ptr<t_candidate_generator> & generator) {
        generators.push_back(generator);
    }
    void add_ranker (const t_const_ptr<t_candidate_ranker> & ranker) {
        rankers.push_back(ranker);
    }

    const t_const_ptr<t_candidate_generator> & get_generator (t_id generator_id) const {
        assert(generator_id >= 0 && generator_id < generators.size());
        return generators[generator_id];
    }
    const t_const_ptr<t_candidate_ranker> & get_ranker (t_id ranker_id) const  {
        assert(ranker_id >= 0 && ranker_id < rankers.size());
        return rankers[ranker_id];
    }

    const t_connection & get_connections () const;


    void add_connection (t_id generator_id,
                         t_id ranker_id) {
        assert(generator_id >= 0 && generator_id < generators.size());
        assert(ranker_id >= 0 && ranker_id < rankers.size());
        connections.push_back(t_connection(generator_id, ranker_id));
    }

    friend std::ostream& operator<<(std::ostream& s, const t_diagnostic_system & ds);

protected:
    std::vector < t_const_ptr < t_candidate_generator > > generators;
    std::vector < t_const_ptr < t_candidate_ranker > > rankers;
    t_connections connections;
};

std::ostream& operator<<(std::ostream& s, const t_diagnostic_system& ds){
    s << "t_diagnostic_system[generators:" << ds.generators << ", ";
    s << "rankers:" << ds.rankers << ", ";
    s << "connections:" << ds.connections << "]";
    return s;
}

class t_algorithm {
    public:
    typedef std::map<std::string, std::string> t_configs;

    t_algorithm (std::string name,
                 t_const_ptr<t_configs> configs):name(name), configs(configs) {
        assert(configs.get() != NULL);
    }

    const std::string & get_name() const {
        return name;
    }

    const t_configs & get_configs() const {
        return *configs;
    }

    static t_ptr<t_configs> new_configs () {
        return t_ptr<t_configs>(new t_configs());
    }

    private:
    std::string name;
    t_const_ptr<t_configs> configs;
};


class t_algorithm_creator {
    public:
    virtual t_const_ptr<t_candidate_generator> generator(const t_algorithm & a) const {
        return t_const_ptr<t_candidate_generator>();
    }

    virtual t_const_ptr<t_candidate_ranker> ranker(const t_algorithm & a) const {
        return t_const_ptr<t_candidate_ranker>();
    }

    virtual ~t_algorithm_creator() {}
};

class t_algorithm_creator_combiner: public t_algorithm_creator {
    public:
    virtual t_const_ptr<t_candidate_generator> generator(const t_algorithm & a) const {
        t_const_ptr<t_candidate_generator> ret;
        BOOST_FOREACH(const t_creators::value_type & e, creators) {
            ret = e->generator(a);
            if(ret)
                return ret;
        }
        return ret;
    }

    virtual t_const_ptr<t_candidate_ranker> ranker(const t_algorithm & a) const {
        t_const_ptr<t_candidate_ranker> ret;
        BOOST_FOREACH(const t_creators::value_type & e, creators) {
            ret = e->ranker(a);
            if(ret)
                return ret;
        }
        return ret;
    }

    void add(t_const_ptr<t_algorithm_creator> c) {
        creators.push_back(c);
    }

    private:
    typedef std::list<t_const_ptr<t_algorithm_creator> > t_creators;
    t_creators creators;
};

class t_similarity_creator : public t_algorithm_creator {
    virtual t_const_ptr<t_candidate_ranker> ranker(const t_algorithm & a) const {
        t_const_ptr<t_candidate_ranker> ret;
        const t_algorithm::t_configs & configs = a.get_configs();

        if(a.get_name() != "similarity")
            return ret;

        t_algorithm::t_configs::const_iterator it = configs.find("type");
        if(it == configs.end())
            return ret;

        string type = it->second;
        if(type == "ochiai")
            ret = t_const_ptr<t_candidate_ranker>(new t_ochiai());
        else if(type == "jaccard")
            ret = t_const_ptr<t_candidate_ranker>(new t_jaccard());

        return ret;
    }
};

class t_mhs_creator: public t_algorithm_creator {
    virtual t_const_ptr<t_candidate_generator> generator (const t_algorithm & a) const {
        t_const_ptr<t_candidate_generator> ret;
        const t_algorithm::t_configs & configs = a.get_configs();

        if(a.get_name() != "mhs")
            return ret;
        ret = t_const_ptr<t_candidate_generator>(new t_mhs());
        //! TODO: Add all options
        return ret;
    }
};


t_const_ptr<t_algorithm::t_configs> parse_configs(const ptree & pt){
    t_ptr<t_algorithm::t_configs> ptr = t_algorithm::new_configs();

    BOOST_FOREACH(const ptree::value_type & e, pt) {
        (*ptr)[e.first] = e.second.get_value<string>();
    }

    return ptr;
}


t_const_ptr<t_algorithm> parse_algorithm (const ptree & pt){
    string name = pt.get<string>("name");

    t_const_ptr<t_algorithm::t_configs> configs;
    try {
        configs = parse_configs(pt.get_child("configs"));
    }
    catch(ptree_bad_path e) {
        configs = t_const_ptr<t_algorithm::t_configs>(new t_algorithm::t_configs());
    }

    return t_const_ptr<t_algorithm>(new t_algorithm(name, configs));
}


t_const_ptr<t_diagnostic_system> parse_diagnostic_system (const ptree & pt,
                                                          const t_algorithm_creator & ac) {
    t_ptr<t_diagnostic_system> dj(new t_diagnostic_system());

    BOOST_FOREACH(const ptree::value_type & e, pt.get_child("generators")) {
        print(e.second);
        t_const_ptr<t_algorithm> a = parse_algorithm(e.second);
        t_const_ptr<t_candidate_generator> cg;
        if(a)
            cg = ac.generator(*a);
        dj->add_generator(cg);
    }

    BOOST_FOREACH(const ptree::value_type & e, pt.get_child("rankers")) {
        print(e.second);
        t_const_ptr<t_algorithm> a = parse_algorithm(e.second);
        t_const_ptr<t_candidate_ranker> cr;
        if(a)
            cr = ac.ranker(*a);
        dj->add_ranker(cr);
    }

    BOOST_FOREACH(const ptree::value_type & e, pt.get_child("connections")) {
        int from = e.second.get("from", -1);
        int to = e.second.get("to", -1);
        dj->add_connection(from, to);
    }


    return dj;
}



int main() {
  ptree pt;
  read_json (std::cin, pt);
  print(pt);
  cout << std::endl;
  t_algorithm_creator_combiner acc;
  acc.add(t_const_ptr<t_algorithm_creator>(new t_mhs_creator()));
  acc.add(t_const_ptr<t_algorithm_creator>(new t_similarity_creator()));

  t_const_ptr<t_diagnostic_system> dj = parse_diagnostic_system(pt, acc);
  cout << *dj;
  return 0;
}
