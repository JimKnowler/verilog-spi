module SPIPeripheral (
    input i_clk,
    input i_reset,

    output reg o_temp
);

always @(posedge i_clk or posedge i_reset)
begin
    if (i_reset)
    begin
        o_temp <= 0;
    end
    else
    begin
        o_temp <= 1;
    end
end

endmodule