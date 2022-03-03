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
    input [2:0] I_color_mode,
    input I_inst_img,
    input I_wr_en_tmp,
    input I_1st_data,
    input I_2nd_data,
    input I_last_data,    
    input I_3c_en,
    input I_8bit_dc,
    input I_cs_toggle,
    output reg O_cs,
    output O_clk,
    output reg O_sdio,
    output reg O_1Data_done,
    output reg O_lastData_done    
    );
    
    reg rClkEn;
    reg [5:0] rCmdRgbCnt;
        
    parameter RGB888=0;
    parameter RGB666=1;
    parameter RGGB5335=2;
    parameter GBRG3553=3;
    
    parameter CMD2C=9'b0_0010_1100;
    parameter CMD3C=9'b0_0011_1100;
    
    wire [5:0] rgbCnt=27;
    wire [26:0] rgbData={27{1'b0}};
    wire [5:0] cmdRgbCnt=36;
    wire [35:0] cmdRgbData={36{1'b0}};
    wire lastDataDone;
    wire I_wr_en;      
    wire I_wr_dis;  
    
    assign O_clk=rClkEn?I_clk:0 ;
    //assign rgbCnt=(I_color_mode==RGB888)?(I_8bit_dc==1?27:25):0;
    assign rgbCnt=(I_color_mode==RGB888)?(I_8bit_dc==1?(3+24):(1+18)):
                        ((I_color_mode==RGGB5335)?(I_8bit_dc==1?(2+16):(1+16)):(I_8bit_dc==1?(2+16):(1+16)));
//    assign rgbCnt=(I_color_mode==RGB888)?(I_8bit_dc==1?27:25):
//                    ((I_color_mode==RGB666)?(I_8bit_dc==1?(3+24):(1+18)):
//                    ((I_color_mode==RGGB5335)?(I_8bit_dc==1?(2+16):(1+16)):(I_8bit_dc==1?(2+16):(1+16))));
    assign rgbData=(I_color_mode==RGB888)?(I_8bit_dc==1   ?   {{1'b1},I_r[7:0],{1'b1},I_g[7:0],{1'b1},I_b[7:0]}   :   {{1'b1},I_r[7:0],I_g[7:0],I_b[7:0]}):
                    ((I_color_mode==RGB666)?(I_8bit_dc==1   ?   {{1'b1},{6'b00_0000},I_r[7:6],{1'b1},I_r[5:2],I_g[7:4],{1'b1},I_g[3:2],I_b[7:2]}   :   {{1'b1},I_r[7:2],I_g[7:2],I_b[7:2]}):
                    ((I_color_mode==RGGB5335)?(I_8bit_dc==1   ?   {{1'b1},I_r[7:3],I_g[7:5],{1'b1},I_g[4:2],I_b[7:3]}   :   {{1'b1},I_r[7:3],I_g[7:2],I_b[7:3]}):
                                              (I_8bit_dc==1   ?   {{1'b1},I_g[4:2],I_b[7:3],{1'b1},I_r[7:3],I_g[7:5]}   :   {{1'b1},I_g[4:2],I_b[7:3],I_r[7:3],I_g[7:5]})
                    )
                    );  
    assign cmdRgbCnt=(I_1st_data) ? (1+8+rgbCnt):(I_3c_en?(1+8+rgbCnt):rgbCnt);//
    assign cmdRgbData=(I_1st_data) ? {{1'b0,CMD2C},rgbData}:(I_3c_en?{{1'b0,CMD3C},rgbData}:rgbData);//
    assign lastDataDone=(I_last_data) ? 1'b1:1'b0;
    assign I_wr_en=(I_inst_img && I_wr_en_tmp) ? 1:0;
    assign I_wr_dis=(I_inst_img && !I_wr_en_tmp) ? 1:0;
    
    
    always@(*)
    begin
        
    end
    
    always@(negedge I_rst or negedge I_clk)
    begin
        if(!I_rst)
            begin
                
            end
    end
    
    always@(negedge I_rst or negedge I_clk)
    begin
        if(!I_rst)
            begin
                O_cs<=1;
                rClkEn<=0;
                O_sdio<=0;
                rCmdRgbCnt<=cmdRgbCnt;
                O_1Data_done<=0;
                O_lastData_done<=0;
            end
        else
            begin
                if(I_wr_en)
                    begin
                        if(I_1st_data || I_2nd_data || I_last_data)
                            begin
                                if(rCmdRgbCnt>1 && !O_1Data_done)
                                    begin
                                        O_cs<=0;
                                        rClkEn<=1;
                                        O_sdio<=cmdRgbData[rCmdRgbCnt-1];
                                        rCmdRgbCnt<=rCmdRgbCnt-1;
                                        O_1Data_done<=0;
                                        O_lastData_done<=0;
                                    end
                                else
                                    begin
                                        O_cs<=0;
                                        rClkEn<=1;
                                        O_sdio<=cmdRgbData[rCmdRgbCnt-1];
                                        rCmdRgbCnt<=cmdRgbCnt;
                                        O_1Data_done<=1;
                                        O_lastData_done<=lastDataDone;
                                    end
                            end
                        else if(I_2nd_data)
                            begin
                                
                            end
                        else if(I_last_data)
                            begin
                                
                            end
                        else
                            begin
                                O_cs<=0;
                                rClkEn<=0;
                                O_sdio<=0;
                                rCmdRgbCnt<=cmdRgbCnt;                                
                            end
                    end
                else if(I_wr_dis)
                    begin
                        O_cs<=1;
                        rClkEn<=0;
                        O_sdio<=0;
                        rCmdRgbCnt<=cmdRgbCnt;
                        //O_1Data_done<=0;
                        //O_lastData_done<=0;
                    end
            end
    end
        
    
endmodule
