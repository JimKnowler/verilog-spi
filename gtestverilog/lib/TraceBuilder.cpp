#include "TraceBuilder.h"

namespace gtestverilog {

    TraceBuilder::TraceBuilder() {

    }

    TraceBuilder::~TraceBuilder() {

    }

    TraceBuilder::operator Trace() {
        finishCurrentPort();

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

        finishCurrentPort();

        currentPort = port;

        return *this;
    }


    TraceBuilder& TraceBuilder::signal(const std::string& stepValues) {
        if (!currentPort) {
            throw std::logic_error("unable to add signal without current port");
        }

        if (stepValues.empty()) {
            throw std::logic_error("unable to add zero-length signal");
        }

        concat();

        for (char c : stepValues) {
            switch (c) {
                case '0':
                case '_':
                    // lo
                    currentSignal.push_back(false);
                    break;
                case '1':
                case '-':
                    // hi
                    currentSignal.push_back(true);
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

        if (stepValues.size() == 0) {
            throw std::logic_error("unable to add zero-length signal");
        }

        concat();

        currentSignal.insert(currentSignal.end(), stepValues.begin(), stepValues.end());

        return *this;
    }

    TraceBuilder& TraceBuilder::repeat(size_t repetitions) {
        applyModifier([=](std::vector<PortValue>& signal) {
            std::vector<PortValue> newSignal;

            for (size_t i=0; i<repetitions; i++) {
                newSignal.insert(newSignal.end(), signal.begin(), signal.end());
            }

            signal = newSignal;
        });

        return *this;
    }

    TraceBuilder& TraceBuilder::repeatEachStep(size_t repetitions) {
        applyModifier([=](std::vector<PortValue>& signal) {
            std::vector<PortValue> newSignal;

            for (const PortValue& value : signal ) {
                for (size_t i=0; i<repetitions; i++) {
                    newSignal.push_back(value);
                }
            }

            signal = newSignal;
        });

        return *this;
    }

    TraceBuilder& TraceBuilder::concat() {
        if (!currentSignal.empty()) {
            currentPort->stepValues.insert(currentPort->stepValues.end(), currentSignal.begin(), currentSignal.end());

            currentSignal.clear();
        }

        return *this;
    }

    TraceBuilder& TraceBuilder::allPorts() {
        finishCurrentPort();

        return *this;
    }

    void TraceBuilder::finishCurrentPort() {
        concat();

        currentPort.reset();
    }

    void TraceBuilder::applyModifier(Modifier modifier) {
        if (currentPort) {
            if (!currentSignal.empty()) {
                modifier(currentSignal);
            } else {
                modifier(currentPort->stepValues);
            }
        } else {
            for (auto& port : ports) {
                modifier(port->stepValues);
            }
        }
    }

    TraceBuilder::Port::Port(const PortDescription& _portDesc) : portDesc(_portDesc) {}
}
