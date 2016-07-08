#ifndef __RUNNER_H_c4614e70c17f3af7db08e92ed486246bfc79c04f__
#define __RUNNER_H_c4614e70c17f3af7db08e92ed486246bfc79c04f__

#include "diagnostic_system.h"
#include "diagnostic_report.h"

namespace diagnostic {

class t_runner
{
    public:
    virtual t_const_ptr<t_diagnostic_report> run (const t_spectrum & spectrum,
                                                  t_const_ptr<t_diagnostic_system> ds) const = 0;

    inline virtual ~t_runner() {}
};


class t_basic_runner : public t_runner {
    public:
    virtual t_const_ptr<t_diagnostic_report> run (const t_spectrum & spectrum,
                                                  t_const_ptr<t_diagnostic_system> ds) const;
};
}


#endif
