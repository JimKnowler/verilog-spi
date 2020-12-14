#include "Trace.h"

#include <string.h>

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

void Trace::renderPort(std::ostream& os, size_t maxPortLabelSize, const PortDescription& portDesc, const std::vector<Step>& steps) {
    uint32_t portId = portDesc.id();

    ConsoleColour::Colour portColour = Trace::getColourForPortId(portId);

    os << ConsoleColour().fg(portColour);

    char buffer[64];
    sprintf(buffer, "  %*s ", int(maxPortLabelSize), portDesc.label());
    os << buffer;

    os << ConsoleColour().fg(ConsoleColour::kBlack).bg(portColour);

    
    for (auto& step : steps) {
        os << (step.port(portDesc) ? "-" : "_");
    }

    os << ConsoleColour().reset();
    os << "\n";
}

std::ostream& operator<<(std::ostream &os, const Trace& trace) {
    os << "\n";
    os << ConsoleColour().reset();

    auto& steps = trace.getSteps();
    
    size_t maxPortLabelSize = trace.getMaxPortLabelSize();

    // limit maximum port label size for our 64 byte buffer
    // note: -4 = 1 (null terminator) + 3 (padding spaces used in printf for right-justified port label)
    maxPortLabelSize = std::min(maxPortLabelSize, size_t(64 - 4));

    renderTimeline(os, maxPortLabelSize + 3, steps.size());

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
    size_t maxPortLabelSize = 10;
    
    for (uint32_t portId=0; portId<32; portId++) {
        if (!hasPort(portId)) {
            continue;
        }

        const PortDescription& portDesc = getPortDescription(portId);
        const size_t labelSize = strlen(portDesc.label());
        maxPortLabelSize = std::max(labelSize, maxPortLabelSize);
    }

    return maxPortLabelSize;
}

void Trace::renderPortDiff(std::ostream& os, char diffCharacter, ConsoleColour::Colour diffColour, size_t maxPortLabelSize, const PortDescription& portDesc, const std::vector<Step>& stepsActual, const std::vector<Step>& stepsExpected) {
    std::vector<size_t> diff;
    for (size_t i = 0; i<stepsExpected.size(); i++) {
        if (stepsExpected[i].port(portDesc) != stepsActual[i].port(portDesc)) {
            diff.push_back(i);
        }
    }

    const size_t traceStartX = maxPortLabelSize + 3;        // note: '+3' as used in renderPort()
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