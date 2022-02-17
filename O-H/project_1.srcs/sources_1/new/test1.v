`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/02/17 09:34:41
// Design Name: 
// Module Name: test1
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


module test1(
input I_clk,
input I_rst,
input [7:0] I_data_in_8,
input I_wr_en,
output reg O_clk,
output reg O_cs,
output reg O_sdio
    );
    
    reg pDataCnt=8;    
    
    always@(posedge I_rst or negedge I_clk)
    begin
        if(I_rst)
            begin
                O_cs<=1'b1;
                O_clk<=1'b0;
                O_sdio<=1'b0;
                pDataCnt<=8;
            end
        else
            begin
                if(I_wr_en>0)
                    begin
                        O_cs<=1'b0;
                        O_clk<=I_clk;
                        O_sdio<=I_data_in_8[pDataCnt-1];
                        pDataCnt<=pDataCnt-1;
                    end
            end
    end
    
    
endmodule
