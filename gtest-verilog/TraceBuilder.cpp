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
        for (const PortValue& value : port->stepValues) {
            Step& step = steps[index];
            
            step.port(port->portDesc) = value;
            
            index ++;
        }
    }

    Trace trace;
    for (auto& step : steps) {
        trace.append(step);
    }

    return trace;
}

TraceBuilder& TraceBuilder::port(const PortDescription& portDesc) {
    auto port = std::make_shared<Port>(portDesc);
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

TraceBuilder& TraceBuilder::signal(const std::initializer_list<uint32_t>& stepValues) {
    if (!currentPort) {
        throw std::logic_error("unable to add signal without current port");
    }

    currentPort->stepValues.insert(currentPort->stepValues.end(), stepValues.begin(), stepValues.end());

    return *this;
}

TraceBuilder& TraceBuilder::repeat(size_t repetitions) {
    if (currentPort) {
        repeat(currentPort->stepValues, repetitions);    
    } else {
        for (auto& port : ports) {
            repeat(port->stepValues, repetitions);
        }
    }

    return *this;
}

TraceBuilder& TraceBuilder::repeatEachStep(size_t repetitions) {
    if (currentPort) {
        repeatEachStep(currentPort->stepValues, repetitions);
    } else {
        for (auto& port : ports) {
            repeatEachStep(port->stepValues, repetitions);
        }
    }

    return *this;
}

TraceBuilder& TraceBuilder::allPorts() {
    currentPort.reset();

    return *this;
}

void TraceBuilder::repeat(std::vector<PortValue>& stepValues, size_t repetitions) {
    std::vector<PortValue> newStepValues;

    for (size_t i=0; i<repetitions; i++) {
        newStepValues.insert(newStepValues.end(), stepValues.begin(), stepValues.end());
    }

    stepValues = newStepValues;
}

void TraceBuilder::repeatEachStep(std::vector<PortValue>& stepValues, size_t repetitions) {
    std::vector<PortValue> newStepValues;

    for (const PortValue& value : stepValues ) {
        for (size_t i=0; i<repetitions; i++) {
            newStepValues.push_back(value);
        }
    }

    stepValues = newStepValues;
}

TraceBuilder::Port::Port(const PortDescription& _portDesc) : portDesc(_portDesc) {}
