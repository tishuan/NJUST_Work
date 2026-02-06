`timescale 1ns / 1ps                                                                 
//*************************************************************************          
                                                                                     
//*************************************************************************          
module inst_rom(                                                                     
    input      [4 :0] addr, // 指令地址                                                  
    output reg [31:0] inst       // 指令                                               
    );                                                                               
                                                                                     
    wire [31:0] inst_rom[19:0];  // 指令存储器，字节地址7'b000_0000~7'b111_1111                
    //------------- 指令编码 ---------|指令地址|--- 汇编指令 -----|- 指令结果 -----//                
    assign inst_rom[ 0] = 32'h24010001; // 00H: addiu $1 ,$0,#1   | $1 = 0000_0001H  
    assign inst_rom[ 1] = 32'h00011840; // 08H: sll  $3 ,$1,#1   | $3 = 0000_0002H   
    assign inst_rom[ 2] = 32'h00612024; // 10H: and $4 ,$1, $3   |$4 = 0000_0000H    
    assign inst_rom[ 3] = 32'h2485000F;// 18H: addi $5 ,$4, $15  |$5 = 0000_000FH    
    assign inst_rom[ 4] = 32'h08000000; // 20H: j                                    
                                                                                     
    //读指令,取4字节                                                                       
    always @(*)                                                                      
    begin                                                                            
        case (addr)                                                                  
            5'd0 : inst <= inst_rom[0 ];                                             
            5'd1 : inst <= inst_rom[1 ];                                             
            5'd2 : inst <= inst_rom[2 ];                                             
            5'd3 : inst <= inst_rom[3 ];                                             
            5'd4 : inst <= inst_rom[4 ];                                             
                                                                                     
            default: inst <= 32'd0;                                                  
        endcase                                                                      
    end  
endmodule                                                                            