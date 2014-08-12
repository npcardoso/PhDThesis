#ifndef __COUNT_SPECTRA_H_06d5fd34e1ffdf10f1925f830cad5b1473cdad22__
#define __COUNT_SPECTRA_H_06d5fd34e1ffdf10f1925f830cad5b1473cdad22__

#include "../types.h"
#include "../structs/spectra.h"


namespace diagnostic {
namespace structs {
class t_count_spectra : public t_basic_spectra {
public:

    inline t_count_spectra () {}

    inline t_count_spectra (t_count component_count,
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
}

#endif
