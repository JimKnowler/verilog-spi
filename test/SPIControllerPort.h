#pragma once

/// @enum SPIControllerPort
/// @brief enumeration of ports on SPIController module
/// @note enumerated port names should match name of ports on 
///       verilated module
/// @todo generate this from verilated module .. python script?
enum SPIControllerPort : uint32_t {
    i_clk,
    o_tx_ready,
    o_spi_clk,
    o_spi_copi
};