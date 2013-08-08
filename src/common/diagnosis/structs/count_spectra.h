#ifndef __DIAGNOSIS_SPECTRA_COUNT_SPECTRA_H__
#define __DIAGNOSIS_SPECTRA_COUNT_SPECTRA_H__

#include "types.h"
#include "diagnosis/structs/spectra_iterator.h"
#include "diagnosis/structs/spectra.h"


namespace diagnosis {
namespace structs {
class t_count_spectra : public t_basic_spectra {
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

    virtual void set_element_count (t_count component_count,
                                    t_count transaction_count);

    t_transaction_id new_transaction ();

    void hit (t_component_id component,
              t_transaction_id transaction,
              t_count count=1,
              bool ignore_unknown_components=false);

    virtual std::ostream & write (std::ostream & out,
                                  const t_spectra_filter * filter=NULL) const;

    virtual std::istream & read (std::istream & in) {
        return read(in, false);
    }

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