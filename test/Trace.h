#pragma once

#include <vector>
#include <ostream>

#include "Step.h"
#include "ConsoleColour.h"

class Trace {
public:
    Trace();

    /// @brief const accessor to steps in the trace
    const std::vector<Step>& getSteps() const;

    /// @brief clear the trace
    void clear();

    /// @brief append a step to the end of the trace
    void append(const Step& step);

    /// @brief retrieve the port mask used by all steps in this trace
    /// @note will return 0 when trace has no steps
    const uint32_t getPortMask() const;

    /// @brief dump state of signal to output stream
    friend std::ostream& operator<<(std::ostream &os, const Trace& trace);

    /// @brief choose a console colour for a port ID
    static ConsoleColour::Colour getColourForPortId(uint32_t portId);

    /// @brief get port description for a port ID
    const PortDescription& getPortDescription(uint32_t portId) const;

    /// @return true if this trace has this port
    bool hasPort(uint32_t portId) const;

private:
    

    std::vector<Step> steps;
    

};
