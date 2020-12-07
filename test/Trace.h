#pragma once

#include <vector>

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
private:
    

    std::vector<Step> steps;
    

};
