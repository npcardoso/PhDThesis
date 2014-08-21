#ifndef __COUNT_SPECTRUM_H_06d5fd34e1ffdf10f1925f830cad5b1473cdad22__
#define __COUNT_SPECTRUM_H_06d5fd34e1ffdf10f1925f830cad5b1473cdad22__

#include "../types.h"
#include "../structs/spectrum.h"


namespace diagnostic {

class t_count_spectrum : public t_basic_spectrum {
public:

    inline t_count_spectrum () {}

    inline t_count_spectrum (t_count component_count,
                            t_count transaction_count) {
        set_count(component_count, transaction_count);
    }

    virtual void set_count (t_count component_count,
                            t_count transaction_count);

    virtual t_count get_activations (t_component_id component,
                                     t_transaction_id transaction) const;

    virtual void set_activations (t_component_id component,
                                  t_transaction_id transaction,
                                  t_count count,
                                  bool ignore_unknown_components=false);


    t_transaction_id new_transaction ();

    void hit (t_component_id component,
              t_transaction_id transaction,
              t_count count=1,
              bool ignore_unknown_components=false);

    virtual std::istream & read (std::istream & in);
    virtual std::istream & read (std::istream & in,
                                 bool has_confidence);

private:
    t_error read_error (std::istream & in) const;

private:
    typedef boost::shared_ptr<t_count[]> t_activity_ptr;
    t_activity_ptr activity;
};
}

#endif
