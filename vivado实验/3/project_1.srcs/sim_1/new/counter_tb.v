`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/04/26 15:52:24
// Design Name: 
// Module Name: counter_tb
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


module counter_tb(    );
   
reg rst;
reg clk;
wire [3:0] count;
 
counter wt(.clk(clk),.rst(rst), .count(count));
 
initial 
begin
  rst <= 0;
  clk <= 0;
  #50 rst <= 1;
  #1000 rst <= 0;
end

always
  #10 clk = ~clk;
endmodule
