#include <gmock/gmock.h>

#include "Trace.h"

namespace matches_trace {
    bool compare(const Trace& a, const Trace& b);
}

MATCHER_P(MatchesTrace, trace, "") {
    if (!matches_trace::compare(arg, trace)) {
        return false;
    }

    return true;
}