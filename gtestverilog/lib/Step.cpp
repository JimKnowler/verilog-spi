#include "Step.h"

namespace gtestverilog {

    Step::Step() {
        portMask = 0;
    }

    size_t Step::getNumPorts() const {
        return ports.size();
    }

    PortValue& Step::port(const PortDescription& portDesc) {
        uint32_t portId = portDesc.id();

        if (!hasPort(&portDesc)) {
            if (getNumPorts() == 32) {
                throw std::logic_error("unable to add more than 32 ports");
            }
        }

        PortValue& value = ports[&portDesc];

        portMask |= (1 << portId);

        return value;
    }

    const PortValue& Step::port(const PortDescription& portDesc) const {
        if (!hasPort(&portDesc)) {
            throw std::logic_error("unknown port");
        }

        auto it = ports.find(&portDesc);
        
        return it->second;
    }

    uint32_t Step::getPortMask() const {
        return portMask;
    }

    bool Step::hasPort(const PortDescription* pPortDesc) const {
        return ports.find(pPortDesc) != ports.end();
    }

    const PortDescription& Step::getPortDescription(uint32_t portId) const {
        for (auto it = ports.begin(); it != ports.end(); it++) {
            if (it->first->id() == portId) {
                return *(it->first);
            }
        }

        throw std::logic_error("unknown port");
    }
}
