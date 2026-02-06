`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/10/30 08:55:09
// Design Name: 
// Module Name: alu
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


module alu(
input   [31:0]  operand1,
input   [31:0]  operand2,
input   [3:0]   aluc,
output  [31:0]  result,
output          cout
    );
    assign{cout,result}=cal(operand1,operand2,aluc);
    function[31:0]cal;
      input[31:0] a,b;
      input [3:0] aluc;
       casex(aluc)
         4'b0000: cal = a+b+7;
         4'b0001: cal = a*b*16;
         4'b0010: cal = a-b-3;
         4'b0011: cal = a&b;
         4'b0100: cal = a^b;
         4'b0101: cal = a|b;
         4'b0110: cal = a/b;
         4'b0111: cal = b<<a[4:0];
       endcase
       endfunction
endmodule
