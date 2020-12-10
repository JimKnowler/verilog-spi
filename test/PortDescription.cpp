#include "PortDescription.h"

PortDescription::PortDescription(uint32_t id, const char* label) : m_id(id), m_label(label) {

}

uint32_t PortDescription::id() const {
    return m_id;
}

const char* PortDescription::label() const {
    return m_label;
}