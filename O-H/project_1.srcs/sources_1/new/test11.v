`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/02/22 15:56:22
// Design Name: 
// Module Name: test11
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


module test11(
    input I_clk,
    input I_rst,
    input [7:0] I_r,
    input [7:0] I_g,
    input [7:0] I_b,
    input [3:0] I_color_mode,
    input I_wr_en,
    input I_1st_data,
    input I_2nd_data,
    input I_last_data,    
    input I_3c_en,
    output reg O_cs,
    output O_clk,
    output reg O_sdio,
    output reg O_1Data_done,
    output reg O_lastData_done    
    );
    
    reg rClkEn;
        
    parameter RGB888=0;
    parameter RGB666=1;
    parameter RGGB5335=2;
    parameter GBRG3553=3;
    
    parameter CMD2C=9'b0_0010_1100;
    parameter CMD3C=9'b0_0011_1100;
    
    wire [5:0] tmpDataCnt;
    wire [23:0] tmpRgbData={24{1'b0}};
    wire [5:0] dataCnt;
    wire [23:0] rgbData={24{1'b0}};
        
    assign O_clk=rClkEn?I_clk:0 ;
    assign tmpDataCnt=(I_color_mode==RGB888)?24:
                    ((I_color_mode==RGB666)?18:
                    ((I_color_mode==RGGB5335)?16:16));
    assign tmpRgbData=(I_color_mode==RGB888)?{I_r[7:0],I_g[7:0],I_b[7:0]}:
                    ((I_color_mode==RGB666)?{I_r[7:2],I_g[7:2],I_b[7:2]}:
                    ((I_color_mode==RGGB5335)?{I_r[7:3],I_g[7:2],I_b[7:3]}:{I_g[4:2],I_b[7:3],I_r[7:3],I_g[7:5]}));
    assign dataCnt=(I_1st_data)?(1+8+1+tmpDataCnt):(I_3c_en?(1+8+1+tmpDataCnt):(1+tmpDataCnt));
    assign rgbData=(I_1st_data)?{{1'b0,CMD2C},tmpRgbData}:(I_3c_en?{{1'b0,CMD3C},tmpRgbData}:{1'b1,tmpRgbData});
    
    always@(posedge I_rst or negedge I_clk)
    begin
        if(!I_rst)
            begin
                
            end
    end
    
    always@(posedge I_rst or negedge I_clk)
    begin
        if(!I_rst)
            begin
                O_cs=1;
                rClkEn=0;
                O_sdio=0;
                O_1Data_done=0;
                O_lastData_done=0;
            end
        else
            begin
                
            end
    end
        
    
endmodule
