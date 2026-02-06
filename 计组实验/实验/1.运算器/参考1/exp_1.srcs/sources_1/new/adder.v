`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/10/29 19:22:26
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


module adder(
    input [31:0] operand1,
    input [31:0] operand2,
    input cin,
    output [31:0] result,
    output cout
);
    // 临时变量，用于存储中间的进位
    wire [32:0] sum_with_carry;

    // 使用Verilog的加法器原语进行加法运算
    // 加法器原语会输出和以及最高位的进位
    assign sum_with_carry = operand1 + operand2 + cin;

    // 将加法器原语的输出赋值给模块的输出端口
    assign result = sum_with_carry[31:0]; // 32位结果
    assign cout = sum_with_carry[32];     // 最高位的进位
endmodule