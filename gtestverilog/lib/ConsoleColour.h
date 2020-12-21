#pragma once

#include <ostream>

namespace gtestverilog {

    /// @brief based on MacOS console colours described here:
    ///  https://stackoverflow.com/questions/33309136/change-color-in-os-x-console-output
    class ConsoleColour {
    public:
        ConsoleColour();

        enum Colour : uint32_t {
            kBlack = 0,
            kRed,
            kGreen,
            kYellow,
            kBlue,
            kMagenta,
            kCyan,
            kWhite,

            kNone
        };

        // set the foreground colour
        ConsoleColour& fg(Colour colour);
        ConsoleColour& fg(uint32_t colour);

        // set the background colour
        ConsoleColour& bg(Colour colour);
        ConsoleColour& bg(uint32_t colour);

        // reset the foreground and background colour
        ConsoleColour& reset();

        friend std::ostream& operator<<(std::ostream& os, const ConsoleColour& consoleColour);

    private:
        Colour colourForeground;
        Colour colourBackground;
    };
}
