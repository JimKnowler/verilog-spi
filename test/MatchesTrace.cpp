#include "MatchesTrace.h"

namespace matches_trace {

    bool compare(const Trace& a, const Trace& b, ::testing::MatchResultListener& listener) {

        auto& stepsA = a.getSteps();
        auto& stepsB = b.getSteps();
        
        const uint32_t sizeA = stepsA.size();
        const uint32_t sizeB = stepsB.size();

        if (sizeA != sizeB) {
            listener << "different number of steps: expected [" << sizeB << "] != actual [" << sizeA << "]\n";
            return false;
        }

        const uint32_t portMaskA = a.getPortMask();
        const uint32_t portMaskB = b.getPortMask();

        const uint32_t portMaskOverlap = portMaskA & portMaskB;

        for (uint32_t portId=0; portId<32; portId++) {
            if (0 == (portMaskOverlap & (1 << portId))) {
                continue;
            }
            
            for (size_t step=0; step < sizeA; step++) {
                const bool valueA = stepsA[step].port(portId);
                const bool valueB = stepsB[step].port(portId);

                if (valueA != valueB) {
                    listener << "different value on " 
                            << ConsoleColour().fg(Trace::getColourForPortId(portId))
                            << "port " << portId
                            << ConsoleColour().reset()
                            << " at step [" << step << "]: expected [" << valueB << "] != actual [" << valueA << "]\n";

                    return false;
                }
                
            }
        }

        return true;
    }
}
