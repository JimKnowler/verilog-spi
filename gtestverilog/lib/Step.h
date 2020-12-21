#pragma once

#include <cstdint>
#include <map>

#include "PortDescription.h"
#include "PortValue.h"

namespace gtestverilog {
    /// @class Step
    /// @brief encapsulate state of ports at a step in a simulation
    class Step {
    public:
        Step(); 

        // @brief accessor for port value
        // @note adds port if it doesn't exist
        PortValue& port(const PortDescription& portDesc);

        // @brief const accessor for port
        // @note throw exception if port doesn't exist
        const PortValue& port(const PortDescription& portDesc) const;

        /// @brief Get the number of ports stored by this step
        size_t getNumPorts() const;

        /// @brief get bitmask showing which ports are in use
        uint32_t getPortMask() const;

        /// @brief get port description for port ID
        const PortDescription& getPortDescription(uint32_t portId) const;

    private:
        bool hasPort(const PortDescription* pPortDesc) const;

        std::map<const PortDescription*, PortValue> ports;

        uint32_t portMask;
    };
}
