#include "Step.h"

Step::Step() {
    portMask = 0;
}

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
    if (!hasPort(portId)) {
        if (getNumPorts() == 32) {
            throw std::logic_error("unable to add more than 32 ports");
        }
    }

    bool& value = ports[portId];

    portMask |= (1 << portId);

    return value;
}

const bool& Step::port(uint32_t portId) const {
    auto it = ports.find(portId);

    if (!hasPort(portId)) {
        throw std::logic_error("unknown port");
    }
    
    return it->second;
}

uint32_t Step::getPortMask() const {
    return portMask;
}

bool Step::hasPort(uint32_t portId) const {
    return ports.find(portId) != ports.end();
}