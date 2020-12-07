#pragma once

#include <vector>

#include "VSPIController.h"
#include "Step.h"

class Trace {
public:
    Trace();

    const std::vector<Step>& getSteps() const;

    void clear();

    void append(const Step& step);
private:
    

    std::vector<Step> steps;
    

};
