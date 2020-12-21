#include <gmock/gmock.h>

#include "Trace.h"

namespace gtestverilog {

    namespace matches_trace {
        bool compare(const Trace& a, const Trace& b, ::testing::MatchResultListener& listener);
    }

    MATCHER_P(MatchesTrace, trace, "") {
        if (!matches_trace::compare(arg, trace, *result_listener)) {
            return false;
        }

        return true;
    }
}