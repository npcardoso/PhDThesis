#ifndef __BASIC_SPECTRUM_H_3a27c6a1340b6d8af51dc0f40c4eb4e2e6b37961__
#define __BASIC_SPECTRUM_H_3a27c6a1340b6d8af51dc0f40c4eb4e2e6b37961__

#include "../structs/spectrum.h"

namespace diagnostic {
class t_basic_spectrum : public t_spectrum {
public:

    t_basic_spectrum ();

    t_basic_spectrum (t_count component_count,
                      t_count transaction_count);

    // Gets

    virtual t_count get_error_count (const t_spectrum_filter * filter=NULL) const;
    virtual t_count get_component_count (const t_spectrum_filter * filter=NULL) const;
    virtual t_count get_transaction_count (const t_spectrum_filter * filter=NULL) const;
    virtual t_error get_error (t_transaction_id transaction) const;

    virtual t_confidence get_confidence (t_transaction_id transaction) const;

    // Sets

    virtual void set_count (t_count component_count,
                            t_count transaction_count);
    virtual void set_error (t_transaction_id transaction,
                            t_error error);
    virtual t_confidence set_confidence (t_transaction_id transaction,
                                         t_confidence confidence);

    // Checks

    virtual bool is_error (t_transaction_id transaction) const;
private:
    typedef std::vector<t_error> t_errors;
    typedef std::vector<t_confidence> t_confidences;
    t_errors errors;
    t_confidences confidences;
    t_count component_count;
    t_count transaction_count;
};
}
#endif