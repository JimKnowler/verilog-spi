#pragma once

#include <cstdint>
#include <vector>

#include "VSPIController.h"

class Trace {
public:
    Trace();

    struct Step {
        // todo: dynamically generate these fields from VSPIController
        uint8_t i_clk;
        uint8_t o_tx_ready;
        uint8_t o_spi_clk;
        uint8_t o_spi_copi;

        void probe(VSPIController& core);
    };

    const std::vector<Step>& getSteps() const;

    void clear();

    void append(const Step& step);
private:
    

    std::vector<Step> steps;
    

};
