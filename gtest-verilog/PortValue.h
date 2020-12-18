#pragma once

#include <variant>
#include <cstdint>

namespace testing_verilog {
    typedef std::variant<bool, uint32_t> PortValue;
}