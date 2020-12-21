#include <gmock/gmock.h>
using namespace testing;

#include "gtestverilog/gtestverilog.h"
using namespace gtestverilog;

namespace {
    PORT_DESCRIPTION(0, test_port_0, 1);
    PORT_DESCRIPTION(1, test_port_1, 1);
    PORT_DESCRIPTION(2, test_port_2, 1);
    PORT_DESCRIPTION(3, test_port_3, 16);
}

TEST(MatchesTrace, ShouldMatchEmptyTraces) {
    Trace trace1;
    Trace trace2;
    
    EXPECT_THAT(trace1, MatchesTrace(trace2));
}

TEST(MatchesTrace, ShouldMatchSingleOverlappingPortWithSameSignal) {
    Trace trace1 = TraceBuilder()
        .port(test_port_1).signal("01");
    Trace trace2 = TraceBuilder()
        .port(test_port_1).signal("01");
    
    EXPECT_THAT(trace1, MatchesTrace(trace2));
}

TEST(MatchesTrace, ShouldMatchNonOverlappingPortsWithDifferentSignals) {
    Trace trace1 = TraceBuilder()
        .port(test_port_1).signal("01");
    Trace trace2 = TraceBuilder()
        .port(test_port_2).signal("10");

    EXPECT_THAT(trace1, MatchesTrace(trace2));
}

TEST(MatchesTrace, ShouldNotMatchSingleOverlappingPortWithDifferentSignal) {
    Trace trace1 = TraceBuilder()
        .port(test_port_1).signal("01");
    Trace trace2 = TraceBuilder()
        .port(test_port_1).signal("10");
    
    EXPECT_THAT(trace1, Not(MatchesTrace(trace2)));
}

TEST(MatchesTrace, ShouldMatchOverlappingPortsWithSameSignalAndIgnoreNonOverlappingPortsWithDifferentSignals) {
    Trace trace1 = TraceBuilder()       
        .port(test_port_0).signal("00")           // non-overlapping
        .port(test_port_1).signal("10");          // overlapping
    Trace trace2 = TraceBuilder()
        .port(test_port_1).signal("10")           // overlapping
        .port(test_port_2).signal("11");          // non-overlapping
    
    EXPECT_THAT(trace1, MatchesTrace(trace2));
}

TEST(MatchesTrace, ShouldNotMatchOverlappingPortsWithSignalsOfDifferentLength) {
    Trace trace1 = TraceBuilder()
        .port(test_port_1).signal("101");
    Trace trace2 = TraceBuilder()
        .port(test_port_1).signal("10");
    
    EXPECT_THAT(trace1, Not(MatchesTrace(trace2)));
}

TEST(MatchesTrace, ShouldMatchOverlappingPortsWithMultibitSignals) {
    Trace trace1 = TraceBuilder()
        .port(test_port_3).signal( {0xffff, 0x1234, 0xabcd });
    Trace trace2 = TraceBuilder()
        .port(test_port_3).signal( {0xffff, 0x1234, 0xabcd } );
    
    EXPECT_THAT(trace1, MatchesTrace(trace2));
}

TEST(MatchesTrace, ShouldNotMatchOverlappingPortsWithDifferentMultibitSignals) {
    Trace trace1 = TraceBuilder()
        .port(test_port_3).signal( {0xffff, 0x1234, 0xabcd });
    Trace trace2 = TraceBuilder()
        .port(test_port_3).signal( {0xffff, 0x1234, 0xdcba } );
    
    EXPECT_THAT(trace1, Not(MatchesTrace(trace2)));
}

TEST(MatchesTrace, ShouldNotMatchOverlappingPortsWithMultibitSignalsOfDifferentLengths) {
    Trace trace1 = TraceBuilder()
        .port(test_port_3).signal( {0xffff, 0x1234, 0xabcd });
    Trace trace2 = TraceBuilder()
        .port(test_port_3).signal( {0xffff, 0x1234, 0xabcd, 0xef12 } );
    
    EXPECT_THAT(trace1, Not(MatchesTrace(trace2)));
}

/// @todo test print nice report to TTY 
///       (by testing underlying matches_trace::compare() directly?)