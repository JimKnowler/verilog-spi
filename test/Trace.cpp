#include "Trace.h"

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

std::ostream& operator<<(std::ostream &os, const Trace& trace) {
    os << "\n";
    const uint32_t portMask = trace.getPortMask();

    auto& steps = trace.getSteps();
    size_t numSteps = steps.size();
        
    // todo: colour each port differently
    for (uint32_t portId=0; portId<32; portId++) {
        if (0 == (portMask & (1 << portId))) {
            continue;
        }

        /// @todo human readable label for each port
        /// @todo scale/timeline at top of trace

        char portLabel[32];
        sprintf(portLabel, "  port %2u: [", portId);
        os << portLabel;

        for (size_t step=0; step < numSteps; step++) {
            // todo: colour hi/lo differently
            os << (steps[step].port(portId) ? "-" : "_");
        }

        os << "]\n";
    }

    return os;
}