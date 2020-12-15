#pragma once

#include <variant>
#include <cstdint>

typedef std::variant<bool, uint32_t> PortValue;
