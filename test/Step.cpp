#include "Step.h"

void Step::probe(VSPIController& core) {
    port(i_clk) = core.i_clk;
    port(o_tx_ready) = core.o_tx_ready;
    port(o_spi_clk) = core.o_spi_clk;
    port(o_spi_copi) = core.o_spi_copi;
}

size_t Step::getNumPorts() const {
    return ports.size();
}

bool& Step::port(uint32_t portId) {
    bool& value = ports[portId];

    return value;
}

const bool& Step::port(uint32_t portId) const {
    auto it = ports.find(portId);

    if (it == ports.end()) {
        throw std::logic_error("unknown port");
    }
    
    return it->second;
}