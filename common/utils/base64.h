#ifndef __COMMON_UTILS_BASE64_H__
#define __COMMON_UTILS_BASE64_H__

#include <string>

std::string base64_encode(unsigned char const* , unsigned int len, bool ignore_leading_zeros=false);
std::string base64_decode(std::string const & s);

#endif
