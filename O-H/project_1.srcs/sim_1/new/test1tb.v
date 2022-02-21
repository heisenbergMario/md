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
    reg I_1st_byte;
    reg I_2nd_byte;
    reg I_last_byte;
    wire O_clk;
    wire O_cs;
    wire O_sdio;
    wire O_1Byte_done;
    wire O_lastByte_done;
    
    reg [7:0] dataNum;
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
        I_wr_en=0;
        I_data_in_8=8'b0101_0011;
        I_1st_byte=0;
        I_2nd_byte=0;
        I_last_byte=0;
        #100         
        I_1st_byte=1;
        I_wr_en=1;
//        while(dataNum)
//            begin
//                @(posedge I_clk) 
//                if(O_1Byte_done==1)
//                    begin
//                        dataNum=dataNum-1;
//                        //I_wr_en=0;
//                        I_1st_byte=0;
//                        I_2nd_byte=1;
//                        I_data_in_8={$random} % 60;
//                        //I_wr_en=1;
//                    end
//            end        
//        I_wr_en=0;
//        I_2nd_byte=0;
//        I_last_byte=1;
//        I_data_in_8=8'b0011_1100;
//        I_wr_en=1;                 
    end   
    

    always@(posedge I_clk) 
        begin
            if(O_1Byte_done==1)
                begin
                    if(dataNum>1)
                        begin
                            dataNum=dataNum-1;
                            //I_wr_en=0;
                            I_1st_byte=0;                            
                            I_2nd_byte=1;
                            I_data_in_8={$random} % 60;
                        end
                    else
                        begin
                            dataNum=dataNum-1;
                            //I_wr_en=0;
                            I_2nd_byte=0;                            
                            I_last_byte=1;
                            I_data_in_8={$random} % 60;
                        end
                end
            if(O_lastByte_done==1)
                begin
                    I_wr_en=0;
                end
        end
        
        
    
//    always @(posedge I_clk) 
//    begin
//        if(O_1Byte_done==1)
//            begin
//                I_wr_en=0;
//                I_1st_byte=0;
//                I_2nd_byte=1;
//                I_data_in_8={$random} % 60;
//                I_wr_en=1;
//            end
//    end
    
    test1 test1_inst(
        .I_clk(I_clk),
        .I_rst(I_rst),
        .I_data_in_8(I_data_in_8),
        .I_wr_en(I_wr_en),
        .I_1st_byte(I_1st_byte),
        .I_2nd_byte(I_2nd_byte),
        .I_last_byte(I_last_byte),
        .O_cs(O_cs),
        .O_clk(O_clk),
        .O_sdio(O_sdio),
        .O_1Byte_done(O_1Byte_done),
        .O_lastByte_done(O_lastByte_done)
        
    );    
    
    
endmodule
