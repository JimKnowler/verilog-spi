#include "Trace.h"

#include <string.h>

namespace gtestverilog {
    Trace::Trace() {
    }

    const std::vector<Step>& Trace::getSteps() const {
        return steps;
    }

    void Trace::clear() {
        steps.clear();
    }

    void Trace::append(const Step& step) {
        if (!steps.empty()) {
            if (steps[0].getPortMask() != step.getPortMask()) {
                throw std::logic_error("steps should have matching ports");
            }
        }
        steps.push_back(step);
    }

    const uint32_t Trace::getPortMask() const {
        if (steps.empty()) {
            return 0;
        } else {
            return steps[0].getPortMask();
        }
    }

    bool Trace::hasPort(uint32_t portId) const {
        return 0 != (getPortMask() & (1 << portId));
    }

    static void renderTimeline(std::ostream& os, size_t x, size_t numSteps) {
        char buffer[64];

        sprintf(buffer, "%*s", int(x), " ");
        os << buffer;

        const int kDividerSize = 5;
        int numDividers = (numSteps + kDividerSize - 1) / kDividerSize;

        for (int i=0; i<numDividers;i++) {
            char divider[8];
            sprintf(divider, "|%-*d", kDividerSize-1, i * kDividerSize);
            os << divider;
        }

        os << "\n";
    }

    static void renderRowForPort(std::ostream& os, size_t maxPortLabelSize, const PortDescription& portDesc, const std::vector<Step>& steps, bool renderPortName, size_t nibbleIndex) {
        uint32_t portId = portDesc.id();

        ConsoleColour::Colour portColour = Trace::getColourForPortId(portId);

        os << ConsoleColour().fg(portColour);

        char buffer[64];
        sprintf(buffer, "  %*s ", int(maxPortLabelSize), renderPortName ? portDesc.label() : "");
        os << buffer;

        os << ConsoleColour().reset();
        if (portDesc.width() > 1) {
            sprintf(buffer, "[%02lu:%02lu] ", ((nibbleIndex + 1) * 4) - 1, (nibbleIndex * 4));
            os << buffer;
        } else {
            os << "....... ";
        }

        os << ConsoleColour().fg(ConsoleColour::kBlack).bg(portColour);
        
        if (portDesc.width() == 1) {
            // single-bit port
            for (auto& step : steps) {
                os << (std::get<bool>(step.port(portDesc)) ? "-" : "_");
            }
        } else  {
            // multi-bit port
            size_t stepIndex = 0;
            while (stepIndex < steps.size()) {
                uint32_t value = std::get<uint32_t>(steps[stepIndex].port(portDesc));

                size_t runLength = 1;
                stepIndex += 1;

                while ((stepIndex < steps.size()) && (value == std::get<uint32_t>(steps[stepIndex].port(portDesc)))) {
                    runLength += 1;
                    stepIndex += 1;
                }

                uint8_t nibble = uint8_t( (value >> (nibbleIndex * 4)) & 0xf);
                sprintf(buffer, "%X", nibble);
                // use null terminator to force limit to a single hex character
                buffer[1] = 0;
                
                if (runLength == 1) {
                    os << "<";
                } else if (runLength == 2) {
                    os << "<" << buffer;
                } else {
                    os << "<" << buffer;
                    for (size_t i = 3; i<runLength; i++) {
                        os << ".";
                    }
                    os << ">";
                }
            }
        }

        os << ConsoleColour().reset();
        os << "\n";
    }

    void Trace::renderPort(std::ostream& os, size_t maxPortLabelSize, const PortDescription& portDesc, const std::vector<Step>& steps) {
        if (portDesc.width() == 1) {
            renderRowForPort(os, maxPortLabelSize, portDesc, steps, true, 0);
        } else {
            size_t numRows = size_t(ceil(portDesc.width() / 4.0f));

            for (size_t displayRowIndex=0; displayRowIndex<numRows; displayRowIndex++) {
                bool renderPortName = (displayRowIndex == 0);

                size_t inverseRowIndex = numRows - displayRowIndex - 1;
                renderRowForPort(os, maxPortLabelSize, portDesc, steps, renderPortName, inverseRowIndex);
            }
        }
    }

    std::ostream& operator<<(std::ostream &os, const Trace& trace) {
        os << "\n";
        os << ConsoleColour().reset();

        auto& steps = trace.getSteps();
        
        size_t maxPortLabelSize = trace.getMaxPortLabelSize();

        renderTimeline(os, maxPortLabelSize + 11, steps.size());

        for (uint32_t portId=0; portId<32; portId++) {
            if (!trace.hasPort(portId)) {
                continue;
            }

            const PortDescription& portDesc = trace.getPortDescription(portId);

            Trace::renderPort(os, maxPortLabelSize, portDesc, steps);
        }

        return os;
    }

    ConsoleColour::Colour Trace::getColourForPortId(uint32_t portId) {
        ConsoleColour::Colour colour = ConsoleColour::Colour(1 + (portId % 7));

        return colour;
    }

    const PortDescription& Trace::getPortDescription(uint32_t portId) const {
        if (steps.empty()) {
            throw std::logic_error("unable to getPortDescription when trace is empty");
        }

        const Step& step = steps[0];
        
        return step.getPortDescription(portId);
    }

    size_t Trace::getMaxPortLabelSize() const {
        const size_t kMinPortLabelSize = 10;

        size_t maxPortLabelSize = kMinPortLabelSize;
        
        for (uint32_t portId=0; portId<32; portId++) {
            if (!hasPort(portId)) {
                continue;
            }

            const PortDescription& portDesc = getPortDescription(portId);
            const size_t labelSize = strlen(portDesc.label());
            maxPortLabelSize = std::max(labelSize, maxPortLabelSize);
        }

        // limit maximum port label size for our 64 byte buffer
        // note: -12 = 1 (null terminator) + 7 (space for [hi:lo]) + 4 (padding spaces used in printf for right-justified port label)
        maxPortLabelSize = std::min(maxPortLabelSize, size_t(64 - 10));

        return maxPortLabelSize;
    }

    void Trace::renderPortDiff(std::ostream& os, char diffCharacter, ConsoleColour::Colour diffColour, size_t maxPortLabelSize, const PortDescription& portDesc, const std::vector<Step>& stepsActual, const std::vector<Step>& stepsExpected) {
        std::vector<size_t> diff;
        for (size_t i = 0; i<stepsExpected.size(); i++) {
            if (stepsExpected[i].port(portDesc) != stepsActual[i].port(portDesc)) {
                diff.push_back(i);
            }
        }

        const size_t traceStartX = maxPortLabelSize + 11;        // note: adding spacing to match spacing used in renderPort()
        for (size_t i=0; i<traceStartX; i++) {
            os << " ";
        }

        size_t outputPos = 0;
        for (size_t diffPos : diff) {
            while (outputPos < diffPos) {
                os << " ";
                outputPos += 1;
            }

            os << ConsoleColour().fg(diffColour) << diffCharacter << ConsoleColour().reset();
            outputPos += 1;
        }

        os << "\n";
    }

    Trace Trace::operator+(const Trace& rhs) const {
        Trace trace;

        if (rhs.steps.size() == 0) {
            trace = *this;

            return trace;
        }

        if (steps.size() == 0) {
            trace = rhs;

            return trace;
        }

        if (getPortMask() != rhs.getPortMask()) {
            throw std::logic_error("different port masks");
        }

        const Step& step = steps[0];
        const Step& stepRhs = rhs.steps[0];
        
        for (uint32_t portId=0; portId<32; portId++) {
            if (!hasPort(portId)) {
                continue;
            }

            if (&step.getPortDescription(portId) != &stepRhs.getPortDescription(portId)) {
                throw std::logic_error("ports from different modules");
            }
        }

        trace.steps.insert(trace.steps.end(), steps.begin(), steps.end());
        trace.steps.insert(trace.steps.end(), rhs.steps.begin(), rhs.steps.end());

        return trace;
    }

    Trace Trace::slice(size_t start, size_t size) {
        Trace trace;

        if (start >= steps.size()) {
            return trace;
        }

        auto st = steps.begin() + start;
        auto et = st + std::min(size, steps.size() - start);

        trace.steps.insert(trace.steps.end(), st, et);

        return trace;
    }
}

// TODO
// - double length of each step in TTY, so that byte value can be displayed if held for a clock cycle
// - configure length of each step in TTY, so that we can zoom in..
// - handle traces that are too long for the screen