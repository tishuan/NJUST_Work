`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/11/13 16:32:50
// Design Name: 
// Module Name: regfile
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


module regfile(
    input             clk,      //时钟控制信号
    input             wen,      //写使能信号，1有效
    input      [4 :0] raddr1,   //第一个读端口的地址
    input      [4 :0] raddr2,   //第二个读端口的地址
    input      [4 :0] waddr,    //一个写端口
    input      [31:0] wdata,    //需要写入的数据
    output  [31:0] rdata1,   //读出的数据1
    output  [31:0] rdata2,   //读出的数据2
    input      [4 :0] test_addr,    //输入的调试地址
    output  [31:0] test_data     //输出调试数据
    );
    //总共8个寄存器
integer i = 0;
reg [8:0] REG_Files[8:0];
	initial//初始化8个寄存器，全为0
        for(i = 0;i < 8;i = i + 1) 
        REG_Files[i]<=0;
	always @ (posedge clk)
	begin
	   if(wen)
	   REG_Files[waddr] <= wdata;
	end
	assign rdata1 = REG_Files[raddr1] ;
	assign rdata2 = REG_Files[raddr2];
	assign test_data  = REG_Files[test_addr];
endmodule
