/// @module Counter
/// @brief  A simple incremental counter in verilog
module Counter(
    input i_clk,
    input i_reset,
    output [15:0] o_value
);

reg [15:0] r_value;

always @(posedge i_clk or posedge i_reset)
begin
    if (i_reset)
    begin
       r_value <= 0; 
    end
    else
    begin
        r_value <= r_value + 1;
    end
end

assign o_value = r_value;

endmodule
