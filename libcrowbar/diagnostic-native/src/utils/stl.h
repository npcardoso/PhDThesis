#ifndef __STL_H_ff311b4acb9621f956385d0ae5f19f74769ef9b0__
#define __STL_H_ff311b4acb9621f956385d0ae5f19f74769ef9b0__

template <class T>
class t_ptr_compare {
public:
    bool operator () (const T lhs, const T rhs) {
        return *lhs < *rhs;
    }
};

#endif