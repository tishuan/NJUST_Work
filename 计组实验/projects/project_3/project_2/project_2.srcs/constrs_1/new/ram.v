`timescale 1ns / 1ps
 
module ram(
    input clk,
    input we,
    input [7:0] addr,
    input [31:0] datain,
    output [31:0] dataout
    );
     
    reg [31:0]  ram[0:127];
    assign dataout  = ram[addr];
    always @(posedge clk) begin
        if(we)  ram[addr] <= datain;
    end
     
    integer i;
    initial begin
        for (i = 0; i < 128; i = i+1)
            ram[i] = 0;
        ram[8'h50] = 32'h70;
        ram[8'h51] = 32'h71;
        ram[8'h52] = 32'h72;
        ram[8'h53] = 32'h73;
        ram[8'h54] = 32'h74;
        ram[8'h55] = 32'h75;
    end
endmodule
