#include "MatchesTrace.h"

namespace gtestverilog {
namespace matches_trace {

    bool compare(const Trace& actual, const Trace& expected, ::testing::MatchResultListener& listener) {
        bool hasMatched = true;

        auto& stepsActual = actual.getSteps();
        auto& stepsExpected = expected.getSteps();
        
        const uint32_t sizeActual = stepsActual.size();
        const uint32_t sizeExpected = stepsExpected.size();

        if (sizeActual != sizeExpected) {
            if (listener.IsInterested()) {
                listener << "different number of steps: expected [" << sizeExpected << "] != actual [" << sizeActual << "]\n";
            }
            return false;
        }

        const uint32_t portMaskActual = actual.getPortMask();
        const uint32_t portMaskExpected = expected.getPortMask();

        const uint32_t portMaskOverlap = portMaskActual & portMaskExpected;

        for (uint32_t portId=0; portId<32; portId++) {
            if (0 == (portMaskOverlap & (1 << portId))) {
                continue;
            }

            const PortDescription& portDesc = actual.getPortDescription(portId);
            
            for (size_t step=0; step < sizeActual; step++) {
                const PortValue& valueActual = stepsActual[step].port(portDesc);
                const PortValue& valueExpected = stepsExpected[step].port(portDesc);

                if (valueActual != valueExpected) {
                    if (listener.IsInterested()) {
                        listener << "\nunexpected signal on " 
                                << ConsoleColour().fg(Trace::getColourForPortId(portId))
                                << portDesc.label()
                                << ConsoleColour().reset()
                                << "\n";

                        size_t maxPortLabelSize = expected.getMaxPortLabelSize();

                        listener << "\n";
                        listener << "Expected:";
                        Trace::renderPortDiff(*(listener.stream()), 'v', ConsoleColour::kGreen, maxPortLabelSize-9, portDesc, stepsExpected, stepsActual);

                        Trace::renderPort(*(listener.stream()), maxPortLabelSize, portDesc, stepsExpected);

                        listener << "  Actual:";
                        Trace::renderPortDiff(*(listener.stream()), 'v', ConsoleColour::kRed, maxPortLabelSize-9, portDesc, stepsExpected, stepsActual);

                        Trace::renderPort(*(listener.stream()), maxPortLabelSize, portDesc, stepsActual);
                    }

                    hasMatched = false;

                    break;
                }
                
            }
        }

        return hasMatched;
    }
    
} // matches_trace
} // gtestverilog