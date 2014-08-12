#ifndef __EXCEPTIONS_H_5b0408edb17e1cc2c8f2cbbf4e43fb6a5da2ac48__
#define __EXCEPTIONS_H_5b0408edb17e1cc2c8f2cbbf4e43fb6a5da2ac48__

#include "../diagnostic_system.h"
#include "algorithm.h"

namespace diagnostic {
namespace configuration {
class t_configuration_exception {
    public:
    inline t_configuration_exception(const std::string message):message(message) {}
    inline std::string get_message(){ return message ;}

    private:
    std::string message;
};
}
}
#endif
