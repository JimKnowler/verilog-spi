#include "PortDescription.h"

namespace gtestverilog {

    PortDescription::PortDescription() : m_id(0xffffffff), m_label("undefined"), m_width(0xffffffff) {

    }

    PortDescription::PortDescription(uint32_t id, const char* label, size_t width) : m_id(id), m_label(label), m_width(width) {

    }

    uint32_t PortDescription::id() const {
        return m_id;
    }

    const char* PortDescription::label() const {
        return m_label;
    }

    size_t PortDescription::width() const {
        return m_width;
    }
}
