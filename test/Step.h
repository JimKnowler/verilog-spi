#pragma once

#include <cstdint>
#include <map>

// todo: remove these
#include "VSPIController.h"
#include "SPIControllerPort.h"


/// @class Step
/// @brief encapsulate state of ports at a step in a simulation
class Step {
public:
    // @brief accessor for port value
    // @note adds port if it doesn't exist
    bool& port(uint32_t portId);

    // @brief const accessor for port
    // @note throw exception if port doesn't exist
    const bool& port(uint32_t portId) const;

    // @todo: template? use generic port ids to get ports
    void probe(VSPIController& core);

    /// @brief Get the number of ports stored by this step
    size_t getNumPorts() const;

private:
    std::map<uint32_t, bool> ports;
};