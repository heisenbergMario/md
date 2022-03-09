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
    //reg I_wr_en;    
    reg I_1st_byte;
    reg I_2nd_byte;
    reg I_last_byte;
    
    reg [7:0] I_r;
    reg [7:0] I_g;
    reg [7:0] I_b;
    reg I_inst_img;
    reg [2:0] I_color_mode;
    reg I_wr_en_tmp;
    reg I_1st_data;
    reg I_2nd_data;
    reg I_last_data;
    reg I_3c_en;
    reg I_8bit_dc;
    reg I_cs_toggle;   
    
    wire O_clk;
    wire O_cs;
    wire O_sdio;
    wire O_1Byte_done;
    wire O_lastByte_done;
    wire O_1Data_done;
    wire O_lastData_done;
    
    reg [7:0] dataNum;
    reg [7:0] randomData;
    
    always #50 I_clk=~I_clk;
    
    initial 
        begin
            I_rst=0;
            #100
            I_rst=1;
        end
    
    initial 
    begin
        
        dataNum=5;        
        I_clk=0;        
        I_wr_en_tmp=0;
        I_data_in_8=8'b0101_0011;
        I_1st_byte=0;
        I_2nd_byte=0;
        I_last_byte=0;
        
        I_r=8'b0011_1100;
        I_g=8'b1111_0000;
        I_b=8'b0000_1111;
        I_color_mode=0;
        I_1st_data=0;
        I_2nd_data=0;
        I_last_data=0;
        I_3c_en=0;
        I_8bit_dc=0;
        I_cs_toggle=0;
        #100         
        I_last_byte=1;
        I_1st_data=1;
        
        I_inst_img=1;
        I_wr_en_tmp=1;       
        
    end   
    

//    always@(posedge I_clk) 
//        begin
//            if(O_1Byte_done==1)
//                begin
//                    if(dataNum>1)
//                        begin
//                            dataNum=dataNum-1;
//                            I_wr_en_tmp<=1;
//                            I_1st_byte<=0;                            
//                            I_2nd_byte<=1;
//                            I_data_in_8<={$random} % 60;
//                        end
//                    else
//                        begin
//                            dataNum<=dataNum-1;
//                            I_wr_en_tmp<=1;
//                            I_2nd_byte<=0;                            
//                            I_last_byte<=1;
//                            I_data_in_8<={$random} % 60;
//                        end
//                end
//            if(O_lastByte_done==1)
//                begin
//                    I_wr_en_tmp<=0;
//                    I_1st_byte<=0;                            
//                    I_2nd_byte<=0;
//                    I_last_byte<=0;
//                    if(({$random}%10)<3)
//                        begin
//                            dataNum<=5;
//                            I_1st_byte<=1;                            
//                            I_2nd_byte<=0;
//                            I_last_byte<=0;
//                            I_data_in_8<={$random} % 60;
//                            I_wr_en_tmp<=1;
//                        end                    
//                end
//        end
        
   always@(posedge I_clk) 
    begin
        if(O_1Data_done==1)
            begin
                if(dataNum>1)
                    begin
                        dataNum=dataNum-1;
                        I_wr_en_tmp<=1;
                        I_1st_byte<=0;                            
                        I_2nd_byte<=1;
                        I_data_in_8<={$random} % 60;
                    end
                else
                    begin
                        dataNum<=dataNum-1;
                        I_wr_en_tmp<=1;
                        I_2nd_byte<=0;                            
                        I_last_byte<=1;
                        I_data_in_8<={$random} % 60;
                    end
            end
        if(O_lastData_done==1)
            begin
                I_wr_en_tmp<=0;
                I_1st_byte<=0;                            
                I_2nd_byte<=0;
                I_last_byte<=0;
                if(({$random}%10)<3)
                    begin
                        dataNum<=5;
                        I_1st_byte<=1;                            
                        I_2nd_byte<=0;
                        I_last_byte<=0;
                        I_data_in_8<={$random} % 60;
                        I_wr_en_tmp<=1;
                    end                    
            end
    end 
    
//    always @(posedge I_clk) 
//    begin
//        if(O_1Byte_done==1)
//            begin
//                I_wr_en_tmp=0;
//                I_1st_byte=0;
//                I_2nd_byte=1;
//                I_data_in_8={$random} % 60;
//                I_wr_en_tmp=1;
//            end
//    end
    
//    test1 test1_inst(
//        .I_clk(I_clk),
//        .I_rst(I_rst),
//        .I_data_in_8(I_data_in_8),
//        .I_inst_img(I_inst_img),
//        .I_wr_en_tmp(I_wr_en_tmp),
//        .I_1st_byte(I_1st_byte),
//        .I_2nd_byte(I_2nd_byte),
//        .I_last_byte(I_last_byte),
//        .O_cs(O_cs),
//        .O_clk(O_clk),
//        .O_sdio(O_sdio),
//        .O_1Byte_done(O_1Byte_done),
//        .O_lastByte_done(O_lastByte_done)        
//    );    
    
       test11 test11_inst(         
         .I_clk(I_clk),
         .I_rst(I_rst),
         .I_r(I_r),
         .I_g(I_g),
         .I_b(I_b),
         .I_color_mode(I_color_mode),
         .I_inst_img(I_inst_img),
         .I_wr_en_tmp(I_wr_en_tmp),
         .I_1st_data(I_1st_data),
         .I_2nd_data(I_2nd_data),
         .I_last_data(I_last_data),    
         .I_3c_en(I_3c_en),
         .I_8bit_dc(I_8bit_dc),
         .I_cs_toggle(I_cs_toggle),
         .O_cs(O_cs),
         .O_clk(O_clk),
         .O_sdio(O_sdio),
         .O_1Data_done(O_1Data_done),
         .O_lastData_done(O_lastData_done)    
     );  
    
    
endmodule
