#pragma once

#include <vector>
#include <ostream>

#include "Step.h"
#include "ConsoleColour.h"

namespace gtestverilog {
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

        /// @brief support for rendering a port
        static void renderPort(std::ostream& os, size_t maxPortLabelSize, const PortDescription& portDesc, const std::vector<Step>& steps);

        /// @brief get the size of the largest port label (characters)
        size_t getMaxPortLabelSize() const;
        
        /// @brief render a diff between two ports, that lines up with output from renderPort()
        static void renderPortDiff(std::ostream& os, char diffCharacter, ConsoleColour::Colour, size_t maxPortLabelSize, const PortDescription& portDesc, const std::vector<Step>& stepsActual, const std::vector<Step>& stepsExpected);

        /// @brief concatenate traces
        Trace operator+(const Trace& rhs) const;

        /// @brief copy a subset of the trace
        /// @param start is index of first step
        /// @param size is maximum number of steps
        Trace slice(size_t start, size_t size);
    private:
        std::vector<Step> steps;
    };

}
