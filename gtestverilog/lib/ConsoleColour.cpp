#include "ConsoleColour.h"

namespace gtestverilog {

    ConsoleColour::ConsoleColour() {
        reset();
    }

    ConsoleColour& ConsoleColour::fg(Colour colour) {
        colourForeground = colour;

        return *this;
    }

    ConsoleColour& ConsoleColour::fg(uint32_t colour) {
        return fg(Colour(colour));
    }

    ConsoleColour& ConsoleColour::bg(Colour colour) {
        colourBackground = colour;

        return *this;
    }

    ConsoleColour& ConsoleColour::bg(uint32_t colour) {
        return bg(Colour(colour));
    }

    ConsoleColour& ConsoleColour::reset() {
        colourBackground = kNone;
        colourForeground = kNone;

        return *this;
    }

    std::ostream& operator<<(std::ostream& os, const ConsoleColour& consoleColour) {
        os << "\x1b[";
        
        bool hasFg = (consoleColour.colourForeground != ConsoleColour::kNone);
        bool hasBg = (consoleColour.colourBackground != ConsoleColour::kNone);

        if (hasFg) {
            if (hasBg) {
                // set foreground and background
                os << (30 + consoleColour.colourForeground) << ";" << (40 + consoleColour.colourBackground);
            } else {
                // set foreground
                os << (30 + consoleColour.colourForeground);
            }
        } else if (hasBg) {
            // set background
            os << (40 + consoleColour.colourBackground);
        } else {
            // reset foreground and background
            os << "0";
        }

        os << "m";

        return os;
    }
}
