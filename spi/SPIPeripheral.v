module SPIPeripheral (
    input           i_clk,
    input           i_reset,

    // receive data
    output          o_rx_dv,        // pulse high for 1 cycle when byte received
    output [7:0]    o_rx_byte,      // received data

    // transmit data
    input           i_tx_dv,        // pulse high for 1 cycle to load tx byte
    input [7:0]     i_tx_byte,      // byte to serialise via CIPO

    // SPI interface
    input           i_spi_clk,      // SPI CLK: clock signal from controller
    output          o_spi_cipo,     // SPI CIPO: tri-state: high-z when cs is positive
    input           i_spi_copi,     // SPI CPOI: only process when cs is negative

    /* verilator lint_off UNUSED */
    input           i_spi_cs_n      // chip select (active low)
    /* verilator lint_on UNUSED */
);

reg [2:0] r_rx_bit_index;
reg [7:0] r_tx_byte;
reg r_rx_dv;
reg [7:0] r_rx_byte;
reg r_rx_buffered_0;
reg r_rx_buffered_1;
reg r_rx_buffered_2;

reg r_active;

reg [2:0] r_tx_bit_index;  // index into byte being serialised
reg r_tx_cipo;             // current bit being serialised to controller

// FPGA Clock domain - receive tx_byte when i_tx_dv is pulsed
always @(posedge i_clk or posedge i_reset)
begin
    if (i_reset)
    begin
        r_tx_byte <= 8'h00;
    end
    else
    begin
        if (i_tx_dv) 
        begin
            r_tx_byte <= i_tx_byte;
        end
    end
end

// SPI clock domain - send serialised data on CIPO
always @(posedge i_spi_clk or posedge i_reset)
begin
    if (i_reset)
    begin
        r_active <= 0;
        r_tx_bit_index <= 3'b111;
        r_tx_cipo <= 0;
    end
    else
    begin
        r_active <= 1;
        r_tx_bit_index <= r_tx_bit_index - 1;
        r_tx_cipo <= r_tx_byte[r_tx_bit_index];
    end
end

// SPI clock domain - read serialised data from COPI
always @(negedge i_spi_clk or posedge i_reset)
begin
    if (i_reset)
    begin
        r_rx_bit_index <= 3'b111;
        r_rx_byte <= 8'h00;
    end
    else
    begin
        r_rx_bit_index <= r_rx_bit_index - 1;
        r_rx_byte[r_rx_bit_index] <= i_spi_copi;

        if (r_rx_bit_index == 0)
        begin
            // start process of RX crossing to 
            // FPGA clock domain
            r_rx_buffered_0 <= 1;
        end
        else if (r_rx_bit_index == 1)
        begin
            // clear the buffered signal in preparation for  
            r_rx_buffered_0 <= 0;
        end
    end
end

// FPGA clock domain - receive RX from spi_clk domain
always @(posedge i_clk or posedge i_reset)
begin
    if (i_reset)
    begin
        r_rx_dv <= 0;
    end
    else
    begin
        r_rx_buffered_1 <= r_rx_buffered_0;
        r_rx_buffered_2 <= r_rx_buffered_1;

        if ((r_rx_buffered_2 == 1'b0) & (r_rx_buffered_1 == 1'b1))
        begin
            // rising edge
            r_rx_dv <= 1;
        end
        else
        begin
            r_rx_dv <= 0;
        end
    end
end

assign o_rx_byte = r_rx_dv ? r_rx_byte : 0;
assign o_rx_dv = r_rx_dv;
assign o_spi_cipo = r_active ? r_tx_cipo : 0;

endmodule