/*
 * SPIPeripheralEcho
 *
 * Test module for SPIPeripheral that echos that last value received
 * 
 */

module SPIPeripheralEcho(
    input           i_clk,
    input           i_reset_n,

    // SPI interface
    input           i_spi_clk,      // SPI CLK: clock signal from controller
    output          o_spi_cipo,     // SPI CIPO: tri-state in top module: high-z when cs is positive
    input           i_spi_copi,     // SPI CPOI: only process when cs is negative
    input           i_spi_cs_n,     // chip select (active low)

    // debug internals
    output          o_debug_rx_buffered_2,
    output          o_debug_rx_buffered_1,
    output          o_debug_rx_buffered_0,
    output [2:0]    o_debug_rx_bit_index,
    output [2:0]    o_debug_tx_bit_index,
    output          o_debug_active,
    output [7:0]    o_debug_rx_byte,
    output          o_debug_rx_dv,
    output          o_debug_tx_dv,
    output [7:0]    o_debug_tx_byte
);

wire [7:0] w_rx_byte;
wire w_rx_dv;

reg [7:0] r_tx_byte;
reg r_tx_dv;

SPIPeripheral spi(
    .i_clk(i_clk),
    .i_reset_n(i_reset_n),

    .o_rx_byte(w_rx_byte),
    .o_rx_dv(w_rx_dv),
    .i_tx_dv(r_tx_dv),
    .i_tx_byte(r_tx_byte),

    .i_spi_clk(i_spi_clk),
    .o_spi_cipo(o_spi_cipo),
    .i_spi_copi(i_spi_copi),
    .i_spi_cs_n(i_spi_cs_n),
    
    .o_debug_rx_buffered_2(o_debug_rx_buffered_2),
    .o_debug_rx_buffered_1(o_debug_rx_buffered_1),
    .o_debug_rx_buffered_0(o_debug_rx_buffered_0),
    .o_debug_rx_bit_index(o_debug_rx_bit_index),
    .o_debug_tx_bit_index(o_debug_tx_bit_index),
    .o_debug_active(o_debug_active)
);

always @(posedge i_clk or negedge i_reset_n)
begin
    if (!i_reset_n)
    begin
        r_tx_byte <= 0;
        r_tx_dv <= 0;
    end
    else
    begin
        if (w_rx_dv == 1)
        begin
            r_tx_byte <= w_rx_byte;
            r_tx_dv <= 1;
        end
        else
        begin
            r_tx_dv <= 0;
        end
    end
end

assign o_debug_rx_byte = w_rx_byte;
assign o_debug_rx_dv = w_rx_dv;
assign o_debug_tx_dv = r_tx_dv;
assign o_debug_tx_byte = r_tx_byte;

endmodule
