#ifndef __COMMON_SERIALIZATION_SERIALIZERS_JSON_H__
#define __COMMON_SERIALIZATION_SERIALIZERS_JSON_H__

#include "serialization/serializer.h"

class t_json_array : public t_element_group_writer {
public:
    DEFINE_BOOST_SHARED_PTRS(t_json_array);

    virtual std::ostream& put (std::ostream & out);
    virtual std::ostream& close (std::ostream & out);
};

class t_json_map : public t_element_group_writer {
public:
    DEFINE_BOOST_SHARED_PTRS(t_json_map);

    virtual std::ostream& put (std::ostream & out);
    virtual std::ostream& close (std::ostream & out);
};

class t_json_observation_serializer : public t_observation_serializer {
public:
    DEFINE_BOOST_SHARED_PTRS(t_json_observation_serializer);

    t_json_observation_serializer ();
    t_json_observation_serializer (t_element_group_writer::t_ptr group);

    virtual bool operator () (std::ostream & out,
                              const instrumentation::t_oracle_observation::t_ptr & obs);
    virtual bool operator () (std::ostream & out,
                              const instrumentation::t_probe_observation::t_ptr & obs);
    virtual bool operator () (std::ostream & out,
                              const instrumentation::t_transaction_observation::t_ptr & obs);

    virtual t_observation_serializer::t_ptr array ();

    virtual void close (std::ostream & out);

private:
    t_element_group_writer::t_ptr group;
    std::ostream& observation_single (std::ostream & out,
                                      const instrumentation::t_observation_single & obs,
                                      t_element_group_writer & grp,
                                      const t_observation_serializer_options & opt);
    std::ostream& observation_window (std::ostream & out,
                                      const instrumentation::t_observation_window & obs,
                                      t_element_group_writer & grp,
                                      const t_observation_serializer_options & opt);
    std::ostream& timestamp (std::ostream & out,
                             t_time_interval t);
    std::ostream& string (std::ostream & out,
                          std::string s);
    std::ostream& key (std::ostream & out,
                       std::string k);
};

class t_json_construct_serializer : public t_construct_serializer {
    virtual bool operator () (std::ostream & out,
                              const instrumentation::t_oracle_construct::t_ptr & ctr);
    virtual bool operator () (std::ostream & out,
                              const instrumentation::t_probe_construct::t_ptr & ctr);
    virtual bool operator () (std::ostream & out,
                              const instrumentation::t_transaction_construct::t_ptr & ctr);

    virtual t_ptr array ();
    virtual ~t_json_construct_serializer ();

    virtual void close ();
};

#endif