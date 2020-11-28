module SPIController
/*
#(
    parameter SPI_MODE = 0,
    parameter CLKS_PER_HALF_BIT = 2
)
*/
(
    input i_clk,
    input i_reset

/*
    // TX (MOSI) Signals
    input [7:0] i_TX_Byte,          // Byte to transmit via MOSI
    input       i_TX_DV,            // Data Valid Pulse with i_TX_Byte
    output reg  o_TX_Ready,         // Set to 1 when ready to transmit next byte

    // RX (MISO) Signals
    output reg          o_RX_DV,    // Data Valid (pulsed for 1 clock cycle)
    output reg [7:0]    o_RX_Byte,  // Byte Received on MISO,

    // SPI Interface
    output reg  o_SPI_Clk,          // SPI Clock
    input       i_SPI_CIPO,         // SPI Controller In, Peripheral Out
    output reg  o_SPI_COPI          // SPI Controller Out, Peripheral In
*/
);

always @(posedge i_clk or posedge i_reset)
begin
end

endmodule
