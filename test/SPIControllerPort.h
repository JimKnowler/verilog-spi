#pragma once

/// @enum SPIControllerPort
/// @brief enumeration of ports on SPIController module
enum SPIControllerPort : uint32_t {
    i_clk,
    o_tx_ready,
    o_spi_clk,
    o_spi_copi
};