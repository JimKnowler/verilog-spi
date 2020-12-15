#pragma once

#include <string>
#include <memory>
#include <vector>

#include "Trace.h"

class TraceBuilder {
public:
    TraceBuilder();
    ~TraceBuilder();

    operator Trace() const;

    // select the port to build
    TraceBuilder& port(const PortDescription& portDesc);

    // append a signal to the current port
    TraceBuilder& signal(const std::string& stepValues);

    // repeat the stepValue pattern recorded so far for
    // the current port
    //   "abcd".repeat(2) => "abcdabcd" 
    TraceBuilder& repeat(size_t repetitions);
    
    // repeat each step of the stepValue pattern recorded so far
    // for the current port
    //   "abcd".repeatEachStep(2) => "aabbccdd"
    TraceBuilder& repeatEachStep(size_t repetitions);

    // apply subsequent modifiers to all ports
    TraceBuilder& allPorts();

private:
    struct Port {
        Port(const PortDescription& portDesc);

        const PortDescription& portDesc;

        std::vector<PortValue> stepValues;
    };

    std::shared_ptr<Port> currentPort;
    std::vector<std::shared_ptr<Port>> ports;

    void repeat(std::vector<PortValue>& stepValues, size_t repetitions);
    void repeatEachStep(std::vector<PortValue>& stepValues, size_t repetitions);
};
