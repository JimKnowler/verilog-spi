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
    TraceBuilder& port(uint32_t portId);

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

private:
    struct Port {
        Port(uint32_t _id);

        uint32_t id;
        // todo: support bit (bool) and multibit (byte/word) values
        std::vector<bool> stepValues;
    };

    std::shared_ptr<Port> currentPort;
    std::vector<std::shared_ptr<Port>> ports;

    void add_port(std::shared_ptr<Port> port);
};
