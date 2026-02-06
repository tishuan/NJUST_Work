`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////

// Design Name:   single_cycle_cpu
// Module Name:   F:/new_lab/6_single_cycle_cpu/tb.v
// Project Name:  single_cycle_cpu
//
// Verilog Test Fixture created by ISE for module: single_cycle_cpu
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module tb;

    // Inputs
    reg clk;
    reg resetn;
    reg [4:0] rf_addr;
    reg [31:0] mem_addr;

    // Outputs
    wire [31:0] rf_data;
    wire [31:0] mem_data;
    wire [31:0] cpu_pc;
    wire [31:0] cpu_inst;

    // Instantiate the Unit Under Test (UUT)
    single_cycle_cpu uut (
        .clk(clk), 
        .resetn(resetn), 
        .rf_addr(rf_addr), 
        .mem_addr(mem_addr), 
        .rf_data(rf_data), 
        .mem_data(mem_data), 
        .cpu_pc(cpu_pc), 
        .cpu_inst(cpu_inst)
    );

    initial begin
        // Initialize Inputs
        clk = 1;
        resetn = 0;
        rf_addr = 0;
        mem_addr = 0;

        // Wait 100 ns for global reset to finish
        #100;
      resetn = 1;
      
        // Add stimulus here
        rf_addr = 1;
        #10;
        rf_addr = 2;
        #10;
        rf_addr = 3;
        #10;       
        rf_addr = 4;
        #10;
        rf_addr = 5;
        #10;  
        rf_addr = 6;
        mem_addr = 32'H00000000;
        #10;
        
    end
    always #5 clk=~clk;
endmodule

