`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/02/17 17:08:55
// Design Name: 
// Module Name: test1tb
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


module test1tb(
    );
    reg I_clk;
    reg I_rst;
    reg [7:0] I_data_in_8;
    reg I_wr_en;
    wire O_clk;
    wire O_cs;
    wire O_sdio;
    
    initial 
    begin
        I_wr_en=0;
        I_clk=0;
        I_data_in_8=8'b0101_0011;
        I_rst=0;
        #100
        I_rst=1;
        #200
        I_wr_en=1;
    end
    
    always #50 I_clk=~I_clk;
    
    test1 test1_inst(
        .I_clk(I_clk),
        .I_rst(I_rst),
        .I_wr_en(I_wr_en),
        .O_cs(O_cs),
        .O_clk(O_clk),
        .O_sdio(O_sdio)
        
    );    
    
    
endmodule
