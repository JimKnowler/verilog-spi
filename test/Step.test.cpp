#include <gtest/gtest.h>

using namespace testing;

#include "SPIControllerGenerated.h"

#include "Step.h"

TEST(Step, ShouldConstruct) {
    Step step;

    ASSERT_EQ(0, step.getNumPorts());
}

TEST(Step, ShouldAddPort) {
    Step step;
    step.port(i_clk) = true;

    ASSERT_EQ(1, step.getNumPorts());
}

TEST(Step, ShouldGetPort) {
    Step step;
    step.port(i_clk) = true;

    ASSERT_EQ(true, step.port(i_clk));
}

TEST(Step, ShouldAddMultiplePorts) {
    Step step;
    step.port(i_clk) = true;
    step.port(o_tx_ready) = false;
    step.port(o_spi_clk) = true;
    step.port(o_spi_copi) = false;

    ASSERT_EQ(4, step.getNumPorts());
    ASSERT_EQ(true, step.port(i_clk));
    ASSERT_EQ(false, step.port(o_tx_ready));
    ASSERT_EQ(true, step.port(o_spi_clk));
    ASSERT_EQ(false, step.port(o_spi_copi));
}

TEST(Step, ShouldHandleConst) {
    Step step;
    step.port(i_clk) = true;

    const Step& constStep = step;
    ASSERT_EQ(1, constStep.getNumPorts());
    ASSERT_EQ(true, constStep.port(i_clk));
}

TEST(Step, ShouldFailIfConstPortDoesNotExist) {
    Step step;
    const Step& constStep = step;
    ASSERT_ANY_THROW(constStep.port(i_clk));
}

TEST(Step, ShouldGetPortMask) {
    Step step;
    ASSERT_EQ(0x00, step.getPortMask());

    step.port(i_clk) = true;
    ASSERT_EQ(0x01, step.getPortMask());
    
    step.port(o_tx_ready) = false;
    step.port(o_spi_clk) = true;
    step.port(o_spi_copi) = false;
    ASSERT_EQ(0x0F, step.getPortMask());
}

TEST(Step, ShouldAddMaximumOf32Ports) {
    Step step;
    for (int i=0; i<32; i++) {
        step.port(i) = true;
    }

    // should throw when adding 32nd port
    ASSERT_ANY_THROW(step.port(32));

    // should be able to access the first 32 ports
    for (int i=0; i<32; i++) {
        ASSERT_EQ(true, step.port(i));
    }
}

// todo
// - registerPort() - here or in Trace?
// - probe - independently of VSPIController header