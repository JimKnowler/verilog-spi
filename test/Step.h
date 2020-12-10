#pragma once

#include <cstdint>
#include <map>

#include "PortDescription.h"

/// @class Step
/// @brief encapsulate state of ports at a step in a simulation
class Step {
public:
    Step();

    // @brief accessor for port value
    // @note adds port if it doesn't exist
    bool& port(const PortDescription& portDesc);
    bool& port(uint32_t portId);

    // @brief const accessor for port
    // @note throw exception if port doesn't exist
    const bool& port(const PortDescription& portDesc) const;
    const bool& port(uint32_t portId) const;

    /// @brief Get the number of ports stored by this step
    size_t getNumPorts() const;

    /// @brief get bitmask showing which ports are in use
    uint32_t getPortMask() const;

private:
    bool hasPort(uint32_t portId) const;

    std::map<uint32_t, bool> ports;

    uint32_t portMask;
};