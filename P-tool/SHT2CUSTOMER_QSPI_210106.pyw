'''
ADD CODE2InitialGuide
20200225: 0xB1修正
'''
import tkinter
from tkinter import ttk
from tkinter import StringVar 
import os
from tkinter import filedialog
import math
import subprocess
import time
import re

top = tkinter.Tk()
top.title("SHT tool")


interface_type = tkinter.StringVar()
canvas_option = tkinter.Canvas(top, borderwidth = 0, relief = 'raised')
      
def btn_OpenFile():
    text_target.delete(0.0,"end")
    SourcePath = tkinter.filedialog.askopenfilename(title = "选择源文件",filetypes = [("text","*.txt"),("All File","*")])
    f1 = open(SourcePath,"r",encoding='UTF-8')
    for line in f1.readlines():
        text_source.insert("insert", line)
    sourceString=text_source.get(1.0,"end");
    fun_SHT2CUSTOMER(sourceString)         
    
def fun_SHT2CUSTOMER(sourceString):   
    lines = str(sourceString).split("\n")
    instruction = entry_instruction_cmd.get()
    instruction_finish = entry_instruction_finish.get()
    delay = entry_delay.get()
    for i in range(0,len(lines)):    
        if(lines[i]!= "\n"):          
            wordTotal = str(lines[i]).split(' ')
            if(wordTotal[0] == "mipi.write"):
                word = str(lines[i]).split('#')
                word = word[0].split(' ')
                length = len(word)  
                #instruction
                index = word[2]#index
                #delay
                numList= re.findall(r"0x",str(word))
                para_length = len(numList) - 2#parameters length
                if(para_length<10):
                    text_target.insert("insert","{"+instruction+", "+index+", "+delay+", "+str(para_length)+",   {")
                elif(para_length>=10 and para_length<100):
                    text_target.insert("insert","{"+instruction+", "+index+", "+delay+", "+str(para_length)+",  {")
                else:
                    text_target.insert("insert","{"+instruction+", "+index+", "+delay+", "+str(para_length)+", {")
                if(para_length==0):
                    text_target.insert("insert", "0x00,")                
                for i in range(0,para_length):#parameters
                    if(i==0):
                        text_target.insert("insert", word[i+3])
                    if(i>0 and word[i+3]!=""):
                        text_target.insert("insert", ","+word[i+3])
                text_target.insert("insert", "}},"+"\n")
    text_target.insert("insert", "{"+instruction_finish+",   0, 0, 0,   {0}},"+"\n")
       
def btn_convert():
    text_target.delete(0.0,"end")
    sourceString=text_source.get(1.0,"end")
    fun_SHT2CUSTOMER(sourceString)  
    
###system level operation
canvas_system = tkinter.Canvas(top, borderwidth = 0, relief = 'raised')
canvas_system.grid(row = 0,column = 0)

cavas_menu = tkinter.Canvas(top, borderwidth = 0, relief = 'raised')
cavas_menu.grid(row = 0,column = 0,columnspan=2)

btn_open = tkinter.Button(cavas_menu,width = 10,height = 1,text = "open",command = btn_OpenFile)
btn_open.grid(row = 0,column = 0)

btn_convert = tkinter.Button(cavas_menu,width = 10,height = 1,text = "convert",command = btn_convert)
btn_convert.grid(row = 0,column = 1)

label_instruction = tkinter.Label(cavas_menu,text = "Instruction")
label_instruction.grid(row = 0, column = 3)
entry_instruction_cmd = tkinter.Entry(cavas_menu,width = 30)
entry_instruction_cmd.grid(row = 0, column = 4)
entry_instruction_cmd.insert(0,"SHM_INST_CMD_WRITE")
entry_instruction_finish = tkinter.Entry(cavas_menu,width = 30)
entry_instruction_finish.grid(row = 1, column = 4)
entry_instruction_finish.insert(0,"SHM_SEQ_FINISH_CODE")

label_delay = tkinter.Label(cavas_menu,text = "Delay")
label_delay.grid(row = 0, column = 6)
entry_delay = tkinter.Entry(cavas_menu,width = 5)
entry_delay.grid(row = 0, column = 7)
entry_delay.insert(0,"1")

text_source = tkinter.Text(top,width = 100,height = 50)
text_source.grid(row = 1, column = 0)
text_source_scrollbar = tkinter.Scrollbar(top)
text_source_scrollbar.grid(row = 1,column = 0,sticky = "n"+"s"+"e")
text_source.config(yscrollcommand = text_source_scrollbar.set)
text_source_scrollbar.config(command = text_source.yview)

text_target = tkinter.Text(top,width = 100,height = 50)
text_target.grid(row = 1, column = 1)
text_target_scrollbar = tkinter.Scrollbar(top)
text_target_scrollbar.grid(row = 1,column = 1,sticky = "n"+"s"+"e")
text_target.config(yscrollcommand = text_target_scrollbar.set)
text_target_scrollbar.config(command = text_target.yview)

top.mainloop()

