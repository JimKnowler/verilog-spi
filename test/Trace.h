#pragma once

#include <vector>
#include <ostream>

#include "VSPIController.h"
#include "Step.h"

class Trace {
public:
    Trace();

    /// @brief const accessor to steps in the trace
    const std::vector<Step>& getSteps() const;

    /// @brief clear the trace
    void clear();

    /// @brief append a step to the end of the trace
    void append(const Step& step);

    /// @brief retrieve the port mask used by all steps in this trace
    /// @note will return 0 when trace has no steps
    const uint32_t getPortMask() const;

    /// @brief dump state of signal to output stream
    friend std::ostream& operator<<(std::ostream &os, const Trace& trace);

private:
    

    std::vector<Step> steps;
    

};
