'''
带注释
'''
import tkinter
from tkinter import ttk
from tkinter import StringVar 
import os
from tkinter import filedialog
import math
import subprocess
from tkinter import scrolledtext

top = tkinter.Tk()
top.title("EDO tool")

splitUnit = 20  
    
def funConvert(event):
    text_Reg_target.delete('0.0','end')
    strSource=text_Reg_source.get('1.0','end')
    #text_Reg_target.insert('insert',strSource)
    lines=strSource.split('\n')
    lineCount=len(lines)
    for i in range(0,lineCount):
        if(lines[i]!=""):
            bytes=lines[i].split()        
            if(bytes[0][0]=="r" and len(bytes[0])==3):
                byteCount=len(bytes)-1
                if(byteCount==0):
                    text_Reg_target.insert('insert',bytes[0])
                    text_Reg_target.insert('insert',"\n")
                else:
                    nextByte=splitUnit
                    while((nextByte-splitUnit)<byteCount):
                        if(nextByte-splitUnit>=20):
                            text_Reg_target.insert('insert',"rB0 ")
                            text_Reg_target.insert('insert',str(hex(nextByte-splitUnit+1))[2:]) 
                            text_Reg_target.insert('insert',"\n")
                        text_Reg_target.insert('insert',bytes[0])
                        for i in range(nextByte-splitUnit,min(len(bytes)-1,nextByte)):
                            text_Reg_target.insert('insert'," ")
                            text_Reg_target.insert('insert',bytes[1+i])                        
                        text_Reg_target.insert('insert',"\n")
                        nextByte=nextByte+splitUnit
                
                    
            else:
                text_Reg_target.insert('insert',lines[i])
                text_Reg_target.insert('insert',"\n")
        
        
###Regs convert

canvas_Regs_convert = tkinter.Canvas(top, borderwidth = 0, relief = 'raised')
canvas_Regs_convert.grid(row = 1,column = 1)

'''
text_Reg_source = tkinter.Text(canvas_Regs_convert,width = 77,height = 50)
text_Reg_source.grid(row = 0, column = 0)
text_Reg_source.bind('<Double-Button-1>',funConvert)
#text_Reg_source.bind('<KeyRelease-Return>',funConvert)

text_Reg_target = tkinter.Text(canvas_Regs_convert,width = 77,height = 50)
text_Reg_target.grid(row = 0, column = 1)
'''

text_Reg_source = scrolledtext.ScrolledText(canvas_Regs_convert,width = 77,height = 50)
text_Reg_source.grid(row = 0, column = 0)
text_Reg_source.bind('<Double-Button-1>',funConvert)

text_Reg_target = scrolledtext.ScrolledText(canvas_Regs_convert,width = 77,height = 50)
text_Reg_target.grid(row = 0, column = 1)




top.mainloop()

