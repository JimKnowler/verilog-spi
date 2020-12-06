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

    // todo: <generate> these port selection functions
    TraceBuilder& i_clk();
    TraceBuilder& o_tx_ready();
    TraceBuilder& o_spi_clk();
    TraceBuilder& o_spi_copi();

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

/*
    // Focus future repeat()/repeatEachStep() requests on the entire trace, instead
    // of on a current port
    TraceBuilder& trace();
    */

private:
    enum class PortId {
        i_clk,
        o_tx_ready,
        o_spi_clk,
        o_spi_copi
    };

    struct Port {
        Port(PortId _id);

        PortId id;
        // todo: support bit (bool) and multibit (byte/word) values
        std::vector<bool> stepValues;
    };

    std::shared_ptr<Port> currentPort;
    std::vector<std::shared_ptr<Port>> ports;

    void add_port(std::shared_ptr<Port> port);
};
