#ifndef __SERIALIZABLE_H_6c62103f2d368633666df7d14ee17e0f148c8d5c__
#define __SERIALIZABLE_H_6c62103f2d368633666df7d14ee17e0f148c8d5c__

#include <iostream>
namespace diagnostic{
class json_serializable {
    virtual std::ostream & json_write (std::ostream & out) const = 0;
};

class json_deserializable {
    virtual std::istream & json_read (std::istream & in) const = 0;
};
}
#endif
