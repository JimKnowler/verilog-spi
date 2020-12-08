#include <gmock/gmock.h>

MATCHER_P(MatchesTrace, trace, "") {
    /// @todo IMPLEMENT THIS!
    /// - use port bit mask to decide which ports are
    ///   populated on both traces & compare them!
    /// - print report to TTY when signal diagram and ptr to issue
    /// - write unit tests to avoid false positive/negatives
    /// - how can we get nice readable port names? 
    ///   - MACRO?
    ///   - python generation

    return false;
}