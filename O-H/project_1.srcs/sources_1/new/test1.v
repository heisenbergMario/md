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
input [7:0]  I_data_in_8,
input I_wr_en,
input I_1st_byte,
input I_2nd_byte,
input I_last_byte,
output reg O_cs,
output  O_clk,
output reg O_sdio,
output reg O_1Byte_done,
output reg O_lastByte_done
    );
    
    reg [3:0] rDataCnt;    
    reg rClkEn;
        
    assign O_clk=rClkEn?I_clk:0;
    
    always@(negedge I_rst or negedge I_clk)
    begin
        if(!I_rst)
            begin                
                O_cs<=1'b1;
                rClkEn=1'b0;
                O_sdio<=1'b0;
                rDataCnt<=9;
                O_1Byte_done=0;
                O_lastByte_done=0;
            end
        else
            begin
                if(I_wr_en)                
                    begin                    
                        //O_cs<=1'b0;
                        //rClkEn<=1;
                        if(I_1st_byte)
                            begin
                                if(rDataCnt==9)
                                    begin
                                        O_cs<=1'b0;   
                                        rClkEn<=1;                                                                                                     
                                        O_sdio=1'b0;
                                        rDataCnt<=rDataCnt-1;
                                        O_1Byte_done=0;        
                                        O_lastByte_done=0; 
                                    end 
                                else if(rDataCnt>1&&!O_1Byte_done)                                 
                                    begin
                                        O_cs<=1'b0;  
                                        rClkEn<=1;   
                                        O_sdio<=I_data_in_8[rDataCnt-1];
                                        rDataCnt<=rDataCnt-1;
                                        O_1Byte_done=0;        
                                        O_lastByte_done=0; 
                                    end
                                else
                                    begin
                                        O_cs<=1'b0;
                                        rClkEn<=1;   
                                        O_sdio<=I_data_in_8[rDataCnt-1]; 
                                        O_1Byte_done=1;        
                                        O_lastByte_done=0;                                      
                                        rDataCnt<=9;
                                    end
                            end
                        else if(I_2nd_byte)
                            begin
                                if(rDataCnt==9)
                                    begin       
                                        O_cs<=1'b0;   
                                        rClkEn<=1;                                                                 
                                        O_sdio=1'b1;
                                        rDataCnt<=rDataCnt-1;
                                        O_1Byte_done=0;  
                                        O_lastByte_done=0;   
                                    end 
                                else if(rDataCnt>1&&!O_1Byte_done)                                 
                                    begin
                                        O_cs<=1'b0;   
                                        rClkEn<=1;   
                                        O_sdio<=I_data_in_8[rDataCnt-1];
                                        rDataCnt<=rDataCnt-1;
                                        O_1Byte_done=0;  
                                        O_lastByte_done=0;   
                                    end
                                else
                                    begin
                                        O_cs<=1'b0;   
                                        rClkEn<=1;   
                                        O_sdio<=I_data_in_8[rDataCnt-1];
                                        rDataCnt<=9;
                                        O_1Byte_done=1;  
                                        O_lastByte_done=0;   
                                    end
                            end
                        else if(I_last_byte)
                            begin
                                if(rDataCnt==9)
                                    begin
                                        O_cs<=1'b0;   
                                        rClkEn<=1;                                                                  
                                        O_sdio=1'b1;
                                        rDataCnt<=rDataCnt-1;
                                        O_1Byte_done=0;
                                        O_lastByte_done=0;    
                                    end 
                                else if(rDataCnt>1&&!O_1Byte_done)                                 
                                    begin         
                                        O_cs<=1'b0;        
                                        rClkEn<=1;                          
                                        O_sdio<=I_data_in_8[rDataCnt-1];
                                        rDataCnt<=rDataCnt-1;
                                        O_1Byte_done=0;
                                        O_lastByte_done=0; 
                                    end
                                else
                                    begin
                                        O_cs<=1'b0;   
                                        rClkEn<=1;   
                                        O_sdio<=I_data_in_8[rDataCnt-1];
                                        rDataCnt<=9;
                                        O_1Byte_done=1;
                                        O_lastByte_done=1; 
                                    end
                            end                
                        else
                            begin
                                O_cs<=1'b0;
                                rClkEn<=0;
                                O_sdio<=1'b0;
                            end
                    end
                else
                    begin                        
                        O_cs<=1'b1;
                        rClkEn<=0;
                        O_sdio<=1'b0;
                        //O_1Byte_done=0;
                        //O_lastByte_done=0;
                    end
            end
    end
    
    
endmodule
