#ifndef __COUNT_SPECTRA_H__
#define __COUNT_SPECTRA_H__

#include "diagnosis/spectra.h"
#include "types.h"

namespace diagnosis {
class t_count_spectra : public t_basic_spectra {
    typedef boost::shared_ptr < t_count[] >t_activity_ptr;
    t_activity_ptr activity;

public:

    inline t_count_spectra () {}

    inline t_count_spectra (t_count component_count,
                            t_count transaction_count) {
        set_element_count(component_count, transaction_count);
    }

    virtual t_count get_count (t_component_id component,
                               t_transaction_id transaction) const;
    virtual void set_count (t_component_id component,
                            t_transaction_id transaction,
                            t_count count,
                            bool ignore_unknown_components=false);

    virtual const t_count& get_activity (t_component_id component,
                                         t_transaction_id transaction) const;

    virtual void set_element_count (t_count component_count,
                                    t_count transaction_count);

    t_transaction_id new_transaction ();

    void hit (t_component_id component,
              t_transaction_id transaction,
              t_count count=1,
              bool ignore_unknown_components=false);

    virtual std::ostream& print (std::ostream & out,
                                 const t_spectra_filter * filter=NULL) const;
    virtual std::istream& read (std::istream & in);
};
}

#endif