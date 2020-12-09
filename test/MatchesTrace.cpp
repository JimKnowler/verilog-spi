#include "MatchesTrace.h"

namespace matches_trace {

    bool compare(const Trace& a, const Trace& b) {

        auto& stepsA = a.getSteps();
        auto& stepsB = b.getSteps();
        
        const uint32_t sizeA = stepsA.size();
        const uint32_t sizeB = stepsB.size();

        if (sizeA != sizeB) {
            printf("MatchesTrace: false - different number of steps: expected [%u] != actual [%u]\n", sizeB, sizeA);
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
                    printf("MatchesTrace: false - different value on portId [%u] at step [%zu]: expected [%u] != actual [%u]\n", portId, step, valueB, valueA);

                    return false;
                }
                
            }
        }

        return true;
    }
}
