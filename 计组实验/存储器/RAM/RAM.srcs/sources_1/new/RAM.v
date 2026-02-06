`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/11/11 21:51:21
// Design Name: 
// Module Name: RAM
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module RAM(
 input clk,
 input write,
 input [7:0] w_addr,
 input [7:0] r_addr,
 input [31:0] datain,
 output [31:0] dataout
 );
 reg [31:0] ram[0:255];
 assign dataout = ram[r_addr];
 always @(posedge clk) begin
 if(write) ram[w_addr] <= datain;
 end
 integer i;
 initial begin
 for (i = 0; i < 256; i = i+1)
 ram[i] = 0;
// ram[8'h50] = 32'h70;
// ram[8'h51] = 32'h71;
// ram[8'h52] = 32'h72;
// ram[8'h53] = 32'h73;
// ram[8'h54] = 32'h74;
// ram[8'h55] = 32'h75;
 end
 endmodule
