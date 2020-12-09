#include <gmock/gmock.h>

#include "MatchesTrace.h"
#include "TraceBuilder.h"

using namespace ::testing;

TEST(MatchesTrace, ShouldMatchEmptyTraces) {
    Trace trace1;
    Trace trace2;
    
    EXPECT_THAT(trace1, MatchesTrace(trace2));
}

TEST(MatchesTrace, ShouldMatchSingleOverlappingPortWithSameSignal) {
    Trace trace1 = TraceBuilder()
        .port(1).signal("01");
    Trace trace2 = TraceBuilder()
        .port(1).signal("01");
    
    EXPECT_THAT(trace1, MatchesTrace(trace2));
}

TEST(MatchesTrace, ShouldMatchNonOverlappingPortsWithDifferentSignals) {
    Trace trace1 = TraceBuilder()
        .port(1).signal("01");
    Trace trace2 = TraceBuilder()
        .port(2).signal("10");

    EXPECT_THAT(trace1, MatchesTrace(trace2));
}

TEST(MatchesTrace, ShouldNotMatchSingleOverlappingPortWithDifferentSignal) {
    Trace trace1 = TraceBuilder()
        .port(1).signal("01");
    Trace trace2 = TraceBuilder()
        .port(1).signal("10");
    
    EXPECT_THAT(trace1, Not(MatchesTrace(trace2)));
}

TEST(MatchesTrace, ShouldMatchOverlappingPortsWithSameSignalAndIgnoreNonOverlappingPortsWithDifferentSignals) {
    Trace trace1 = TraceBuilder()       
        .port(0).signal("00")           // non-overlapping
        .port(1).signal("10");          // overlapping
    Trace trace2 = TraceBuilder()
        .port(1).signal("10")           // overlapping
        .port(2).signal("11");          // non-overlapping
    
    EXPECT_THAT(trace1, MatchesTrace(trace2));
}

TEST(MatchesTrace, ShouldNotMatchOverlappingPortsWithSignalsOfDifferentLength) {
    Trace trace1 = TraceBuilder()
        .port(1).signal("101");
    Trace trace2 = TraceBuilder()
        .port(1).signal("10");
    
    EXPECT_THAT(trace1, Not(MatchesTrace(trace2)));
}

/// @todo test print nice report to TTY 
///       (by testing underlying matches_trace::compare() directly?)