`timescale 1ns / 1ps
//*************************************************************************

//*************************************************************************
module inst_rom(
    input      [4 :0] addr, // 指令地址
    output reg [31:0] inst       // 指令
    );

    wire [31:0] inst_rom[19:0];  // 指令存储器，字节地址7'b000_0000~7'b111_1111
    //------------- 指令编码 ---------|指令地址|--- 汇编指令 -----|- 指令结果 -----//

    assign inst_rom[ 0] = 32'h24010004; // 00H: addiu $1 ,$0,#4  | $1 = 0000_0004H      I
    assign inst_rom[ 1] = 32'h00011040; // 04H: sll   $2 ,$1,#1   | $2 = 0000_0008H     I
    assign inst_rom[ 2] = 32'h00011842; // 0CH: srl   $3 ,$1,#1   | $3 = 0000_0002H
    assign inst_rom[ 3] = 32'h00622023; // 10H: subu  $4 ,$3,$2   | $4 = 0000_0006H
    assign inst_rom[ 4] = 32'h00622824; // 1CH: and   $5 ,$3,$2   | $5 = FFFF_FFF3H
    assign inst_rom[ 5] = 32'h00623026; // 20H: xor   $6 ,$3,$2   | $6 = 0000_0011H
    assign inst_rom[ 6] = 32'h08000000; // 4CH: j     00H         | 跳转指令00H
	
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

            default: inst <= 32'd0;
        endcase
    end
endmodule