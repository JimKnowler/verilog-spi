/// @module Counter
/// @brief  A simple incremental counter in verilog
module Counter(
    input i_clk,
    output [15:0] o_value
);

reg [15:0] r_value;

always @(posedge i_clk)
begin
    r_value <= r_value + 1;
end

assign o_value = r_value;

endmodule
