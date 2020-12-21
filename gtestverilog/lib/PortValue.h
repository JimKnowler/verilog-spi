#pragma once

#include <variant>
#include <cstdint>

namespace gtestverilog {
    typedef std::variant<bool, uint32_t> PortValue;
}