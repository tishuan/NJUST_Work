`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/10/30 17:47:25
// Design Name: 
// Module Name: adder
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


module adder(a,b,op,r); input [31:0] a;
input [31:0] b;
input [2:0] op;

output [31:0] r;
    assign r = cal(.a(a), .b(b), .op(op));
    function [31:0] cal;
        input [31:0] a;
        input [31:0] b;
        input [2:0] op;
        case(op)
            3'b000 :cal = a+b;
            3'b001 :cal = a-b;
            3'b010 :cal = a*b;
            3'b011 :cal = a/b;
            3'b100 :cal = a&b;
            3'b101 :cal = a|b;
            3'b110 :cal = a^b;
            3'b111 :cal = a<<4;
        endcase
    endfunction
endmodule
