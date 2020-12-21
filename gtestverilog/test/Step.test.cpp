#include <gtest/gtest.h>
using namespace testing;

#include "gtestverilog/gtestverilog.h"
using namespace gtestverilog;

namespace {
    PORT_DESCRIPTION(0, test_port_0, 1);
    PORT_DESCRIPTION(1, test_port_1, 1);
    PORT_DESCRIPTION(2, test_port_2, 1);
    PORT_DESCRIPTION(3, test_port_3, 1);
}

TEST(Step, ShouldConstruct) {
    Step step;

    ASSERT_EQ(0, step.getNumPorts());
}

TEST(Step, ShouldAddPort) {
    Step step;
    step.port(test_port_0) = true;

    ASSERT_EQ(1, step.getNumPorts());
}

TEST(Step, ShouldGetPort) {
    Step step;
    step.port(test_port_0) = true;

    ASSERT_EQ(true, std::get<bool>(step.port(test_port_0)));
}

TEST(Step, ShouldAddMultiplePorts) {
    Step step;
    step.port(test_port_0) = true;
    step.port(test_port_1) = false;
    step.port(test_port_2) = true;
    step.port(test_port_3) = false;

    ASSERT_EQ(4, step.getNumPorts());
    ASSERT_EQ(true, std::get<bool>(step.port(test_port_0)));
    ASSERT_EQ(false, std::get<bool>(step.port(test_port_1)));
    ASSERT_EQ(true, std::get<bool>(step.port(test_port_2)));
    ASSERT_EQ(false, std::get<bool>(step.port(test_port_3)));
}

TEST(Step, ShouldHandleConst) {
    Step step;
    step.port(test_port_0) = true;

    const Step& constStep = step;
    ASSERT_EQ(1, constStep.getNumPorts());
    ASSERT_EQ(true, std::get<bool>(constStep.port(test_port_0)));
}

TEST(Step, ShouldFailIfConstPortDoesNotExist) {
    Step step;
    const Step& constStep = step;
    ASSERT_ANY_THROW(constStep.port(test_port_0));
}

TEST(Step, ShouldGetPortMask) {
    Step step;
    ASSERT_EQ(0x00, step.getPortMask());

    step.port(test_port_0) = true;
    ASSERT_EQ(0x01, step.getPortMask());
    
    step.port(test_port_1) = false;
    step.port(test_port_2) = true;
    step.port(test_port_3) = false;
    ASSERT_EQ(0x0F, step.getPortMask());
}

TEST(Step, ShouldAddMaximumOf32Ports) {
    Step step;
    PortDescription portDescs[33];
    for (int i=0; i<32; i++) {
        PortDescription& portDesc = portDescs[i];
        portDesc = PortDescription(i, "temp test port", 1);

        step.port(portDesc) = true;
    }

    // should throw when adding 32nd port
    PortDescription portDesc32(32, "temp test port", 1);
    ASSERT_ANY_THROW(step.port(portDesc32));

    // should be able to access the first 32 ports
    for (int i=0; i<32; i++) {
        ASSERT_EQ(true, std::get<bool>(step.port(portDescs[i])));
    }
}

TEST(Step, ShouldGetPortDescription) {
    Step step;
    step.port(test_port_0) = true;
    step.port(test_port_3) = false;

    EXPECT_EQ(&test_port_0, &step.getPortDescription(0));
    EXPECT_EQ(&test_port_3, &step.getPortDescription(3));
}

TEST(Step, ShouldFailToGetPortDescriptionForUnknownPortId) {
Step step;
    step.port(test_port_0) = true;
    step.port(test_port_3) = false;

    ASSERT_ANY_THROW(step.getPortDescription(1));
    ASSERT_ANY_THROW(step.getPortDescription(2));
    ASSERT_ANY_THROW(step.getPortDescription(4));
}