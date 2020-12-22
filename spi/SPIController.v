module SPIController
(
    input i_clk,
    input i_reset,

    // TX (COPI) Signals
    input [7:0]  i_tx_byte,         // Byte to transmit via COPI
    input        i_tx_dv,           // Data Valid Pulse with i_tx_byte
    output       o_tx_ready,        // Set to 1 when ready to transmit next byte

    // RX (MISO) Signals
    output       o_rx_dv,           // Data Valid (pulsed for 1 clock cycle)
    output [7:0] o_rx_byte,         // Byte Received on CIPO

    // SPI Interface
    output       o_spi_clk,         // SPI Clock
    input        i_spi_cipo,        // SPI CIPO : Controller In, Peripheral Out   
    output       o_spi_copi         // SPI COPI : Controller Out, Peripheral In
);

reg [7:0] r_tx_byte;
reg [7:0] r_spi_clk_edges;          // track number of spi clk edges left to transmit
reg [2:0] r_tx_bit_count;           // track the current bit that is being transmitted
reg r_tx_ready;                     // hi when ready to send, lo while sending
reg r_spi_clk;                      // SPI clock
reg r_spi_copi;                     // controller out peripheral in - serial data
reg r_rx_dv;                        // set hi for one clock tick when data is valid
reg [7:0] r_rx_byte;                // byte being received from CIPO
reg [2:0] r_rx_bit_count;           // track the current bit that is being received

reg r_is_busy;                      // set to 1 while busy sending/receiving

// generate SPI clock correct number of times when DV pulse comes
always @(posedge i_clk or posedge i_reset)
begin
    
    if (i_reset)
    begin
        r_tx_ready <= 1'b0;
        r_spi_clk <= 1'b0;             // default to IDLE state
        r_spi_clk_edges <= 0;
        r_rx_dv <= 1'b0;   
        r_is_busy <= 0;

    end
    else
    begin
        r_rx_dv <= 1'b0;

        if (i_tx_dv)
        begin
            // start transmitting a byte
            r_tx_ready <= 1'b0;
            r_spi_clk_edges <= 16;      // 8 bits * 2 edges (trailing + leading)

            r_is_busy <= 1;
        end
        else if (r_spi_clk_edges > 0)
        begin
            // transmitting a byte
            r_tx_ready <= 1'b0;
            r_spi_clk_edges <= r_spi_clk_edges - 1;
            r_spi_clk <= ~r_spi_clk;
        end
        else 
        begin
            // ready to start a new transmission
            r_tx_ready <= 1'b1;

            if (r_is_busy)
            begin
                // finish a transmission
                r_rx_dv <= 1'b1;
                r_is_busy <= 1'b0;
            end
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
        r_spi_copi <= 1'b0;
        r_tx_bit_count <= 3'b111;       // send most significant bit first (bit 7)
    end
    else
    begin
        r_tx_bit_count <= r_tx_bit_count - 1;
        r_spi_copi <= r_tx_byte[r_tx_bit_count];
    end
end

// capture CIPO data
always @(posedge o_spi_clk or posedge i_reset) 
begin
    if (i_reset)
    begin
        r_rx_bit_count <= 3'b111;       // receive most significant bit first (bit 7)
        r_rx_byte <= 8'h00;
    end
    else
    begin
        r_rx_bit_count <= r_rx_bit_count - 1;
        r_rx_byte[r_rx_bit_count] <= i_spi_cipo;
    end
end

assign o_spi_copi = r_spi_copi & ~o_tx_ready;
assign o_spi_clk = r_spi_clk;
assign o_tx_ready = r_tx_ready;
assign o_rx_dv = r_rx_dv;
assign o_rx_byte = r_rx_dv ? r_rx_byte : 8'h00;


endmodule
