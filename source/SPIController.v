module SPIController
/*
#(
    parameter SPI_MODE = 0,
    parameter CLKS_PER_HALF_BIT = 1
)
*/
(
    input i_clk,
    input i_reset,

    // TX (COPI) Signals
    input [7:0] i_tx_byte,          // Byte to transmit via COPI
    input       i_tx_dv,            // Data Valid Pulse with i_tx_byte
    output reg  o_tx_ready          // Set to 1 when ready to transmit next byte

    // RX (MISO) Signals
/*
    output reg          o_RX_DV,    // Data Valid (pulsed for 1 clock cycle)
    output reg [7:0]    o_RX_Byte,  // Byte Received on MISO,

    // SPI Interface
    output reg  o_SPI_Clk,          // SPI Clock
    input       i_SPI_CIPO,         // SPI Controller In, Peripheral Out
    output reg  o_SPI_COPI          // SPI Controller Out, Peripheral In
*/
);

reg [7:0] r_tx_byte;

always @(posedge i_clk or posedge i_reset)
begin
    
    if (i_reset)
    begin
        o_tx_ready <= 1;
        r_tx_byte <= 0;
    end
    else
    begin
        if (i_tx_dv)
        begin
            r_tx_byte <= i_tx_byte;
            o_tx_ready <= 0;
        end
        else 
        begin
            r_tx_byte <= r_tx_byte + 1;
        end
    end
end

endmodule
