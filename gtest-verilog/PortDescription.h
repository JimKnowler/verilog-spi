#pragma once

#include <cstdint>
#include <cstdlib>

#define PORT_DESCRIPTION(id, name, width) PortDescription name(id, #name, width)

class PortDescription {
public:
    PortDescription();
    PortDescription(uint32_t id, const char* label, size_t width);

    uint32_t id() const;
    const char* label() const;
    size_t width() const;
    
private:
    uint32_t m_id;
    const char* m_label;
    size_t m_width;
};

