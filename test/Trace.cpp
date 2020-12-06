#include "Trace.h"

Trace::Trace() {
}

const std::vector<Trace::Step>& Trace::getSteps() const {
    return steps;
}

void Trace::clear() {
    steps.clear();
}

void Trace::append(const Step& step) {
    steps.push_back(step);
}

void Trace::Step::probe(VSPIController& core) {
    i_clk = core.i_clk;
    o_tx_ready = core.o_tx_ready;
    o_spi_clk = core.o_spi_clk;
    o_spi_copi = core.o_spi_copi;
}