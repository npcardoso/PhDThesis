#ifndef __COMMON_UTILS_RGB_H__
#define __COMMON_UTILS_RGB_H__

#include <iostream>

class t_rgb {
public:
    t_rgb (float r, float g, float b, float a=1);

    float r, g, b, a;
};

namespace std {
ostream & operator << (ostream & out, const t_rgb & rgb);
}
#endif