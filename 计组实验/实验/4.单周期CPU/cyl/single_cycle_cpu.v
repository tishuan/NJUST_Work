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
    assign inst_rom[ 0] = 32'h24010004; // 00H: addiu $1 ,$0,#4   | $1 = 0000_0100H
    assign inst_rom[ 1] = 32'h24020008; // 04H: addiu $2 ,$0,#8  | $2 = 0000_1000H
    assign inst_rom[ 2] = 32'h00413821; // 08H: addu  $7 ,$2,$1   | $7 = 0000_1100H
    assign inst_rom[ 3] = 32'h00022042; // 0CH: srl   $4 ,$2,#1   | $4 = 0000_0100H srl右移
    assign inst_rom[ 4] = 32'h00243825; // 10H: or    $7 ,$1,$4   | $7 = 0000_0100H
    assign inst_rom[ 5] = 32'h14240003; // 14H: bne   $1,$2,#3   | 不跳转
    assign inst_rom[ 6] = 32'h08000000; // 18H: j     00H         | 跳转指令00H

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
            5'd8 : inst <= inst_rom[8 ];
            5'd9 : inst <= inst_rom[9 ];
            5'd10: inst <= inst_rom[10];
            5'd11: inst <= inst_rom[11];
            5'd12: inst <= inst_rom[12];
            5'd13: inst <= inst_rom[13];
            5'd14: inst <= inst_rom[14];
            5'd15: inst <= inst_rom[15];
            5'd16: inst <= inst_rom[16];
            5'd17: inst <= inst_rom[17];
            5'd18: inst <= inst_rom[18];
            5'd19: inst <= inst_rom[19];
            default: inst <= 32'd0;
        endcase
    end
endmodule