#include "TraceBuilder.h"

TraceBuilder::TraceBuilder() {

}

TraceBuilder::~TraceBuilder() {

}

TraceBuilder::operator Trace() const {
    std::vector<Step> steps;

    if (!ports.empty()) {
        size_t length = ports[0]->stepValues.size();

        for (size_t i=0; i<length; i++) {
            steps.push_back(Step());
        }
    }

    // iterate through ports and write to the steps
    for (auto& port : ports) {
        if (port->stepValues.size() != steps.size()) {
            throw std::logic_error("all ports must have signals with the same number of steps");
        }

        size_t index = 0;
        for (bool value : port->stepValues) {
            Step& step = steps[index];
            step.port(port->id) = value;
            
            index ++;
        }
    }

    Trace trace;
    for (auto& step : steps) {
        trace.append(step);
    }

    return trace;
}

TraceBuilder& TraceBuilder::port(uint32_t portId) {
    auto port = std::make_shared<Port>(portId);
    ports.push_back(port);
    currentPort = port;

    return *this;
}


TraceBuilder& TraceBuilder::signal(const std::string& stepValues) {
    if (!currentPort) {
        throw std::logic_error("unable to add signal without current port");
    }

    for (char c : stepValues) {
        switch (c) {
            case '0':
            case '_':
                // lo
                currentPort->stepValues.push_back(false);
                break;
            case '1':
            case '-':
                // hi
                currentPort->stepValues.push_back(true);
                break;
            default:
                // unknown char
                throw std::logic_error("unable to add signal with unknown char");
                break;
        }
    }
    return *this;
}

TraceBuilder& TraceBuilder::repeat(size_t repetitions) {
    std::vector<bool>& stepValues = currentPort->stepValues;

    std::vector<bool> newStepValues;

    for (size_t i=0; i<repetitions; i++) {
        newStepValues.insert(newStepValues.end(), stepValues.begin(), stepValues.end());
    }

    stepValues = newStepValues;

    return *this;
}

TraceBuilder& TraceBuilder::repeatEachStep(size_t repetitions) {
    std::vector<bool>& stepValues = currentPort->stepValues;

    std::vector<bool> newStepValues;

    for (bool bit : stepValues ) {
        for (size_t i=0; i<repetitions; i++) {
            newStepValues.push_back(bit);
        }
    }

    stepValues = newStepValues;

    return *this;
}

TraceBuilder::Port::Port(uint32_t _id) : id(_id) {}
