`timescale 1ns / 1ps
//*************************************************************************
//   > 文件名: inst_rom.v
//   > 描述  ：异步指令存储器模块，采用寄存器搭建而成，类似寄存器堆
//   >         内嵌好指令，只读，异步读

//*************************************************************************
module inst_rom(
    input      [4 :0] addr, // 指令地址
    output reg [31:0] inst       // 指令
    );

    wire [31:0] inst_rom[19:0];  // 指令存储器，字节地址7'b000_0000~7'b111_1111
    //------------- 指令编码 ---------|指令地址|--- 汇编指令 -----|- 指令结果 -----//
    assign inst_rom[ 0] = 32'h24010005; // 00H: addiu $1 ,$0,#5   | $1 = 0000_0005H    1号寄存器加5
    assign inst_rom[ 1] = 32'h24020002; // 04H: addiu $2 ,$0,#2   | $2 = 0000_0002H    2号寄存器加2
    assign inst_rom[ 2] = 32'h00411824; // 08H: and   $3,$2,$1    | $3 = 0000_0000H    3号寄存器为1、2号寄存器&&
    assign inst_rom[ 3] = 32'h00412025; // 0CH: or    $4 ,$2,$1   | $4 = 0000_0007H    4号寄存器为1、2号寄存器||
    assign inst_rom[ 4] = 32'h00012840; // 04H: sll   $5 ,$1,#1   | $5 = 0000_000AH
    assign inst_rom[ 5] = 32'h08000000; // 1CH: j     00H         | 跳转指令00H

    //读指令,取4字节
    always @(*)
    begin
        case (addr)
            5'd0 : inst <= inst_rom[0 ];
            5'd1 : inst <= inst_rom[1 ];
            5'd2 : inst <= inst_rom[2 ];
            5'd3 : inst <= inst_rom[3 ];
            5'd4 : inst <= inst_rom[4 ];
            5'd5 : inst <= inst_rom[5 ];
            5'd6 : inst <= inst_rom[6 ];
            5'd7 : inst <= inst_rom[7 ];
            default: inst <= 32'd0;
        endcase
    end
endmodule