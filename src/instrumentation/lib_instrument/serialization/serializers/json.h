#ifndef __COMMON_SERIALIZATION_SERIALIZERS_JSON_H__
#define __COMMON_SERIALIZATION_SERIALIZERS_JSON_H__

#include "instrumentation/serialization/serializer.h"

namespace instrumentation {
class t_json_array : public t_element_group_writer {
public:
    virtual std::ostream & put (std::ostream & out);
    virtual std::ostream & close (std::ostream & out);
};

class t_json_map : public t_element_group_writer {
public:
    virtual std::ostream & put (std::ostream & out);
    virtual std::ostream & close (std::ostream & out);
};

class t_json_observation_serializer : public t_observation_serializer {
public:
    t_json_observation_serializer ();
    t_json_observation_serializer (t_ptr<t_element_group_writer> group);

    virtual bool operator () (std::ostream & out,
                              const t_ptr<t_oracle_observation> & obs);
    virtual bool operator () (std::ostream & out,
                              const t_ptr<t_probe_observation> & obs);
    virtual bool operator () (std::ostream & out,
                              const t_ptr<t_transaction_observation> & obs);

    virtual t_ptr<t_observation_serializer> array ();

    virtual void close (std::ostream & out);

private:
    t_ptr<t_element_group_writer> group;
    std::ostream & observation_single (std::ostream & out,
                                       const t_observation_single & obs,
                                       t_element_group_writer & grp,
                                       const t_observation_serializer_options & opt);
    std::ostream & observation_window (std::ostream & out,
                                       const t_observation_window & obs,
                                       t_element_group_writer & grp,
                                       const t_observation_serializer_options & opt);
    std::ostream & timestamp (std::ostream & out,
                              t_time_interval t);
    std::ostream & string (std::ostream & out,
                           std::string s);
    std::ostream & key (std::ostream & out,
                        std::string k);
};

class t_json_construct_serializer : public t_construct_serializer {
    virtual bool operator () (std::ostream & out,
                              const t_ptr<t_oracle_construct> & ctr);
    virtual bool operator () (std::ostream & out,
                              const t_ptr<t_probe_construct> & ctr);
    virtual bool operator () (std::ostream & out,
                              const t_ptr<t_transaction_construct> & ctr);

    virtual t_ptr<t_construct_serializer> array ();
    virtual ~t_json_construct_serializer ();

    virtual void close ();
};
}
#endif