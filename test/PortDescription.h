#pragma once

#include <cstdint>

#define PORT_DESCRIPTION(id, name) PortDescription name(id, #name)

class PortDescription {
public:
    PortDescription(uint32_t id, const char* label);

    uint32_t id() const;

    const char* label() const;
    
private:
    uint32_t m_id;
    const char* m_label;
};

