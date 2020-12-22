module SPIPeripheral (
    input           i_clk,
    input           i_reset,

    // receive data
    output          o_rx_dv,        // pulse high for 1 cycle when byte received
    output [7:0]    o_rx_byte,      // received data

    // transmit data
    input           i_tx_dv,        // pulse high for 1 cycle to load tx byte
    input [7:0]     i_tx_byte,      // byte to serialise via CIPO

/* verilator lint_off UNUSED */
    // SPI interface
    input           i_spi_clk,      // SPI CLK: clock signal from controller
    output          o_spi_cipo,     // SPI CIPO: tri-state: high-z when cs is positive
    input           i_spi_copi,     // SPI CPOI: only process when cs is negative
    input           i_spi_cs_n      // chip select (active low)
/* verilator lint_on UNUSED */
);

/* verilator lint_off UNUSED */
reg [7:0] r_tx_byte;
/* verilator lint_on UNUSED */
reg r_rx_dv;
reg [7:0] r_rx_byte;

reg r_active;

reg [2:0] r_bit_index;
reg r_cipo;

always @(posedge i_clk or posedge i_reset)
begin
    if (i_reset)
    begin
        r_tx_byte <= 8'h00;
        r_rx_dv <= 0;
        r_rx_byte <= 8'h00;
    end
    else
    begin
        if (i_tx_dv) 
        begin
            r_tx_byte <= i_tx_byte;
        end
    end
end

always @(posedge i_spi_clk or posedge i_reset)
begin
    if (i_reset)
    begin
        r_active <= 0;
        r_bit_index <= 3'b111;
    end
    else
    begin
        r_active <= 1;

        r_bit_index <= r_bit_index - 1;
        r_cipo <= r_tx_byte[r_bit_index];
    end
end

assign o_rx_byte = r_rx_byte;
assign o_rx_dv = r_rx_dv;
assign o_spi_cipo = r_active ? r_cipo : 0;

endmodule