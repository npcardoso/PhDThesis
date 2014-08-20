#include "rgb.h"
#include <iomanip>
#include <cassert>

t_rgb::t_rgb (float r, float g, float b, float a) {
    assert(r >= 0);
    assert(g >= 0);
    assert(b >= 0);
    assert(a >= 0);
    assert(r <= 1);
    assert(g <= 1);
    assert(b <= 1);
    assert(a <= 1);
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}

namespace std {
ostream & operator << (ostream & out, const t_rgb & rgb) {
    ios::fmtflags flags = out.flags();
    char fill = out.fill('0');


    out << std::hex;
    out << std::setw(2) << (unsigned) (255 * rgb.r);
    out << std::setw(2) << (unsigned) (255 * rgb.g);
    out << std::setw(2) << (unsigned) (255 * rgb.b);
    out << std::setw(2) << (unsigned) (255 * rgb.a);

    out.fill(fill);
    out.flags(flags);
    return out;
}
}