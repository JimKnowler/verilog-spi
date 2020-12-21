#pragma once

#include <string>
#include <memory>
#include <vector>

#include "Trace.h"

namespace gtestverilog {

    class TraceBuilder {
    public:
        TraceBuilder();
        ~TraceBuilder();

        operator Trace();

        /// @brief select the port to build
        TraceBuilder& port(const PortDescription& portDesc);

        /// @brief append a single bit signal to the current port
        /// @example TraceBuilder().port(myPort).signal("100110")
        /// @example TraceBuilder().port(myPort).signal("-__--_")
        /// @note subsequent modifiers will be applied only to this signal, until concat() or allPorts() are called 
        TraceBuilder& signal(const std::string& stepValues);

        /// @brief append a new signal to the current port
        /// @note subsequent modifiers will be applied only to this signal, until concat() or allPorts() are called
        TraceBuilder& signal(const std::initializer_list<uint32_t>& stepValues);

        /// @brief repeat the current signal being added to the current port
        /// @example TraceBuilder().port(myPort).signal("abcd").repeat(2) => "abcdabcd" 
        TraceBuilder& repeat(size_t repetitions);
        
        /// @brief repeat each step of the current signal being added to the current port
        /// @example TraceBuilder().port(myPort).signal("abcd").repeatEachStep(2) => "aabbccdd"
        TraceBuilder& repeatEachStep(size_t repetitions);

        /// @brief concatenate all signals so far on current port
        /// @note subsequent modifiers will be applied to all signals added so far to the current port
        TraceBuilder& concat();

        /// @brief apply subsequent modifiers to all ports
        TraceBuilder& allPorts();

    private:
        typedef std::function<void(std::vector<PortValue>&)> Modifier;

        struct Port {
            Port(const PortDescription& portDesc);

            const PortDescription& portDesc;

            std::vector<PortValue> stepValues;
        };

        std::vector<PortValue> currentSignal;
        std::shared_ptr<Port> currentPort;
        std::vector<std::shared_ptr<Port>> ports;

        void finishCurrentPort();

        void applyModifier(Modifier modifer);
    };

}
