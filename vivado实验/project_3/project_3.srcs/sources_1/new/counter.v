`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/04/24 22:19:04
// Design Name: 
// Module Name: counter
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

module counter(clk,rst,count);
input clk,rst;
output [3:0] count;
reg [3:0] q;
assign count = q;
always@(posedge clk)
begin
  if(!rst)
    q <= 0;
  else if(q <= 4'd0)
    q <= 9;
  else
    q <= q - 1;
end
endmodule
