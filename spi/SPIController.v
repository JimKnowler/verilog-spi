module SPIController
(
    input i_clk,
    input i_reset,

    // TX (COPI) Signals
    input [7:0] i_tx_byte,          // Byte to transmit via COPI
    input       i_tx_dv,            // Data Valid Pulse with i_tx_byte
    output reg  o_tx_ready,         // Set to 1 when ready to transmit next byte

    // RX (MISO) Signals
/*
    output reg          o_RX_DV,    // Data Valid (pulsed for 1 clock cycle)
    output reg [7:0]    o_RX_Byte,  // Byte Received on MISO,
*/
    // SPI Interface
    output reg  o_spi_clk,           // SPI Clock
/*
    input       i_SPI_CIPO,         // SPI Controller In, Peripheral Out
*/    
    output reg  o_spi_copi          // SPI Controller Out, Peripheral In
);

reg [7:0] r_tx_byte;
reg [7:0] r_spi_clk_edges;                // track number of spi clk edges left to transmit
reg [2:0] r_tx_bit_count;           // track the current bit that is being transmitted

// generate SPI clock correct number of times when DV pulse comes
always @(posedge i_clk or posedge i_reset)
begin
    
    if (i_reset)
    begin
        o_tx_ready <= 1'b0;
        o_spi_clk <= 1'b0;             // default to IDLE state
        r_spi_clk_edges <= 0;
    end
    else
    begin
        if (i_tx_dv)
        begin
            o_tx_ready <= 1'b0;
            r_spi_clk_edges <= 16;      // 8 bits * 2 edges (trailing + leading)
        end
        else if (r_spi_clk_edges > 0)
        begin
            o_tx_ready <= 1'b0;
            // current transmitting a byte
            
            r_spi_clk_edges <= r_spi_clk_edges - 1;
            o_spi_clk <= ~o_spi_clk;
        end
        else 
        begin
            // ready to start a new transmission
            o_tx_ready <= 1'b1;
        end
    end
end

// register r_tx_byte when i_tx_dv is pulsed
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

// generate COPI data
always @(posedge o_spi_clk or posedge i_reset) 
begin
    if (i_reset)
    begin
        o_spi_copi <= 1'b0;
        r_tx_bit_count <= 3'b111;       // send most significant bit first (bit 7)
    end
    else
    begin
        r_tx_bit_count <= r_tx_bit_count - 1;
        o_spi_copi <= r_tx_byte[r_tx_bit_count];
    end
end



endmodule
