#ifndef __DIAGNOSIS_SPECTRA_RANDOMIZER_RANDOMIZER_H__
#define __DIAGNOSIS_SPECTRA_RANDOMIZER_RANDOMIZER_H__

namespace diagnosis {
template < class T >
class t_spectra_randomizer {
public:
    virtual void randomize (T & spectra) const = 0;
};
}

#endif