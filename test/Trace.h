#pragma once

#include <cstdint>
#include <vector>

#include "VSPIController.h"

class Trace {
public:
    uint64_t tickCount;
    
    struct Signals {
        uint8_t i_clk;
        uint8_t o_tx_ready;
        uint8_t o_spi_clk;
        uint8_t o_spi_copi;

        void probe(VSPIController& core) {
            i_clk = core.i_clk;
            o_tx_ready = core.o_tx_ready;
            o_spi_clk = core.o_spi_clk;
            o_spi_copi = core.o_spi_copi;
        }
    };

    struct Tick {
        uint64_t index = 0;
        Signals risingEdge;
        Signals fallingEdge;
    };

    std::vector<Tick> ticks;

    void clear() {
        ticks.clear();
        tickCount = 0;
    }
private:

};
