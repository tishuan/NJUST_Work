`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 
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


module alu(a,b,op,r);
    input [31:0] a;
    input [31:0] b;
    input [2:0] op;
    output [31:0] r;
    assign r=cal(.a(a),.b(b),.op(op));
    
    function [31:0] cal;
		input [31:0] a;
		input [31:0] b;
		input [2:0] op;
		case(op)
			3'b000: cal = a+b;//加
			3'b001: cal = a-b;//减
			3'b010: cal = a*b;//乘
			3'b011: cal = a/b;//除
			3'b100: cal = a&b;//与
			3'b101: cal = a|b;//或
			3'b110: cal = a^b;//异或
			3'b111: cal = a<<b[4:0];//a左移b位
		endcase
	endfunction
    
endmodule
