`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/03/29 16:26:16
// Design Name: 
// Module Name: encode8_3_tb
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


module encode8_3_tb(

    );
    reg[7:0] in;
    wire[2:0] out;
    encode8_3 u0(.in(in),.out(out));
    always begin
        in=8'b00000001;#20;
        in=8'b00000010;#20;
        in=8'b00000100;#20;
        in=8'b00001000;#20;
        in=8'b00010000;#20;
        in=8'b00100000;#20;
        in=8'b01000000;#20;
        in=8'b10000000;#20;
    end
endmodule
