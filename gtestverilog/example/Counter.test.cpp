#include <gtest/gtest.h>
using namespace testing;

#include "gtestverilog/gtestverilog.h"
using namespace gtestverilog;

#include "gtestverilog/CounterTestBench.h"
using namespace countertestbench;

namespace {
    class Counter : public ::testing::Test {
    public:
        void SetUp() override {
        }
        
        void TearDown() override {
        }

        CounterTestBench testBench;
    };
}

TEST_F(Counter, ShouldConstruct) {
}

TEST_F(Counter, ShouldReset) {
    testBench.reset();

    ASSERT_EQ(testBench.core().o_value, 0);
}

TEST_F(Counter, ShouldIncrement) {
    testBench.reset();
    testBench.tick();

    const Trace traceExpected = TraceBuilder()
        .port(i_reset_n).signal( "01").repeatEachStep(2)
        .port(i_clk).signal( "10" ).repeat(2)
        .port(o_value).signal( {0, 1} ).repeatEachStep(2);

    ASSERT_THAT(testBench.trace, MatchesTrace(traceExpected));
}

TEST_F(Counter, ShouldIncrementRepeatedly) {
    testBench.reset();
    testBench.tick(10);

    const Trace traceExpected = TraceBuilder()
        .port(i_reset_n).signal( "01111111111").repeatEachStep(2)
        .port(i_clk).signal( "10" ).repeat(11)
        .port(o_value).signal( {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10} ).repeatEachStep(2);

    ASSERT_THAT(testBench.trace, MatchesTrace(traceExpected));
}
