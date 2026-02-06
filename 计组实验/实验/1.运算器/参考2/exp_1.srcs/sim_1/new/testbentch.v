`timescale 1ns / 1ps   //仿真单位时间为1ns，精度为1ps
module testbench;

    // Inputs
    reg [31:0] operand1;
    reg [31:0] operand2;
    reg [3:0]cin;

    // Outputs
    wire [31:0] result;
    wire cout;
    // Instantiate the Unit Under Test (UUT)
    alu uut (
    //需要补充的部分
    .operand1(operand1),
    .operand2(operand2),
    .aluc(cin),
    .result(result),
    .cout(cout)
    );
    initial begin
        // Initialize Inputs
        operand1 = 5;
        operand2 = 1;
        cin = 4'b0100;
        // Wait 100 ns for global reset to finish
        #100;
        // Add stimulus here
    end
endmodule


