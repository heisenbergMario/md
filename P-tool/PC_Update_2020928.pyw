import tkinter
from tkinter import ttk
from tkinter import StringVar 
import os
from tkinter import filedialog
import math
import subprocess
import os
import sys
import time
import shutil
import win32wnet
import threading

top = tkinter.Tk()
top.title("PC update Tool")

def refresh():
    global pcNum     
    global ngPC
    ngPC=[]
    entry_account.configure(state="disable")
    entry_password.configure(state="disable")
    btn_refresh.configure(state="disable")    
    for i in range(len(checkbtn_names)):
        pcNum = str("\\\\"+checkbtn_names[i]+"\D") 
        try:                        
            win32wnet.WNetAddConnection2(0,None,pcNum,None,entry_account.get(),entry_password.get())
        except Exception as err: 
            print(err.args[0])  
            # Connected
            if err.args[0]==1219:
                print("already connected")
                #win32wnet.WNetCancelConnection2(pcNum,0,0)              
                #win32wnet.WNetAddConnection2(0,None,pcNum,None,entry_account.get(),entry_password.get())
            # No PC
            if err.args[0]==53:
                print("no net path")                 
            # Wrong account or password
            
            
        if(os.path.exists(pcNum)):
            print(str(pcNum+" connect OK"))
            checkbtns[checkbtn_names[i]].select()            
            log_text.config(state="normal")
            log_text.insert("0.0",time.strftime("%H:%M:%S",time.localtime())+" [Connect] "+checkbtn_names[i]+"  success\n")            
            log_text.config(state="disable")
            btn_openTarget.configure(state="normal")
            btn_delete_file.configure(state="normal")
            btn_delete_folder.configure(state="normal")
        else:            
            print(str(pcNum+" connect NG"))
            checkbtns[checkbtn_names[i]].deselect()
            log_text.config(state="normal")
            log_text.insert("0.0",time.strftime("%H:%M:%S",time.localtime())+" [Connect] "+checkbtn_names[i]+"  fail\n")  
            log_text.config(state="disable")     
            ngPC.append(checkbtn_names[i])
    print(ngPC)
    log_text.config(state="normal")
    log_text.insert('0.0',">>>\n")
    log_text.config(state="disable")
    btn_refresh.configure(state="normal")
    entry_account.configure(state="normal")
    entry_password.configure(state="normal")    

def btn_CONNECT():
    T = threading.Thread(target=refresh)
    T.start()

def btn_OpenTargetFile():
    global targetPath
    targetPath=""
    global pcNum 
    global ngPC    
    entry_TargetFile.delete(0,"end")
    targetPath = tkinter.filedialog.askdirectory(title = "选择目标位置",initialdir = pcNum)
    if(len(targetPath)>0):
        btn_copy_file.configure(state="normal")
        btn_copy_folder.configure(state="normal")
        entry_TargetFile.insert(0,targetPath)  
        print(targetPath)
        print(targetPath[6])

def btn_COPY_file():
    global SourcePath       
    global targetPath
    global ngPC
    SourcePath = tkinter.filedialog.askopenfilenames(title = "选择源文件",initialdir = pcNum)
    if(len(SourcePath)>0): 
        log_source_file.config(state="normal")    
        log_text.config(state="normal")    
        log_source_file.delete(0.0,"end")
        for i in range(len(SourcePath)):
            log_source_file.insert("insert",os.path.basename(SourcePath[i]))
            log_source_file.insert("insert","\n")
        for i in range(len(checkbtn_names)):   
            if(checkbtn_names[i] not in ngPC):
                pcPath = list(entry_TargetFile.get())
                pcPath[6] = str(i+1)
                pcPath="".join(pcPath)
                print(pcPath)
                if not (os.path.exists(pcPath)): 
                    try:
                        os.makedirs(pcPath)    
                    except Exception as e:
                        print(e)
                        log_text.insert("0.0",time.strftime("%H:%M:%S",time.localtime())+" [PATH ERROR]   "+pcPath+"\n")
                        #break
                if(os.path.exists(pcPath)):
                    for j in range(len(SourcePath)):                             
                        shutil.copy(SourcePath[j], pcPath)
                        log_text.insert("0.0",time.strftime("%H:%M:%S",time.localtime())+" [Copy]   "+pcPath+"'  success\n")   
            else:
                print(checkbtn_names[i]+"not connected")
        log_text.insert("0.0",">>>\n")
        log_source_file.config(state="normal")
        log_text.config(state="normal")
    
def btn_COPY_folder():
    global SourcePath        
    global targetPath
    global ngPC
    SourcePath = tkinter.filedialog.askdirectory(title = "选择源文件夹",initialdir = pcNum)
    print(SourcePath)
    if(len(SourcePath)>0): 
        log_source_file.config(state="normal")
        log_text.config(state="normal")
        log_source_file.delete(0.0,"end")
        log_source_file.insert("insert",os.path.basename(SourcePath))           
        for i in range(len(checkbtn_names)): 
            if(checkbtn_names[i] not in ngPC):
                pcPath = list(targetPath)
                pcPath[6] = str(i+1)
                pcPath="".join(pcPath)
                print(pcPath)
                pcPath=pcPath+"/"+os.path.basename(SourcePath)
                print(pcPath)
                if not(os.path.exists(pcPath)):
                    try:
                        os.makedirs(pcPath) 
                    except Exception as e:
                        print(e)
                        log_text.insert("0.0",time.strftime("%H:%M:%S",time.localtime())+" [PATH ERROR]   "+pcPath+"\n")
                        break            
                if not os.path.exists(pcPath):
                    print("create pcPath")
                    os.makedirs(pcPath)
                if os.path.exists(SourcePath):
                    print("delete pcPath")
                    shutil.rmtree(pcPath)            
                
                shutil.copytree(SourcePath, pcPath)
                log_text.insert("0.0",time.strftime("%H:%M:%S",time.localtime())+" [Copy]   "+pcPath+"'  success\n")           
            else:
                print(checkbtn_names[i]+"not connected")
        log_text.insert("0.0",">>>\n")
        log_source_file.config(state="normal")
        log_text.config(state="normal")
            
def btn_DELETE_file():
    global ngPC
    deletePath = tkinter.filedialog.askopenfilenames(title = "删除目标文件",initialdir = pcNum)    
    if(len(deletePath)>0):
        log_source_file.config(state="normal")
        log_text.config(state="normal")
        log_source_file.delete(0.0,"end")
        for i in range(len(deletePath)):
            log_source_file.insert("insert",os.path.basename(deletePath[i]))
            log_source_file.insert("insert","\n")
        for i in range(len(checkbtn_names)):
            if(checkbtn_names[i] not in ngPC):
                for j in range(len(deletePath)):
                    pcPath = list(deletePath[j])
                    pcPath[6] = str(i+1)
                    pcPath="".join(pcPath)        
                    if os.path.exists(pcPath):                
                        os.remove(pcPath)
                        log_text.insert("0.0",time.strftime("%H:%M:%S",time.localtime())+" [Delete]   '"+pcPath+"'  success\n")
                    else:
                        log_text.insert("0.0",time.strftime("%H:%M:%S",time.localtime())+" [PATH ERROR]   "+pcPath+"\n")
            else:
                print(checkbtn_names[i]+"not connected")
        log_text.insert("0.0",">>>\n")
        log_source_file.config(state="normal")
        log_text.config(state="normal")
    
def btn_DELETE_folder():
    global ngPC
    deletePath = tkinter.filedialog.askdirectory(title = "删除目标文件夹",initialdir = pcNum)
    if(len(deletePath)>0):
        log_source_file.config(state="normal")
        log_text.config(state="normal")
        log_source_file.delete(0.0,"end")
        log_source_file.insert("insert",os.path.basename(deletePath))  
        for i in range(len(checkbtn_names)):
            if(checkbtn_names[i] not in ngPC):
                pcPath = list(deletePath)
                pcPath[6] = str(i+1)
                pcPath="".join(pcPath)        
                if os.path.exists(pcPath):                
                    shutil.rmtree(pcPath)
                    log_text.insert("0.0",time.strftime("%H:%M:%S",time.localtime())+" [Delete]   '"+pcPath+"'  success\n")
                else:
                    log_text.insert("0.0",time.strftime("%H:%M:%S",time.localtime())+" [PATH ERROR]   "+pcPath+"\n")
            else:
                print(checkbtn_names[i]+"not connected")
        log_text.insert("0.0",">>>\n")
        log_source_file.config(state="normal")
        log_text.config(state="normal")
    
def btn_CLEAR():
        log_text.config(state="normal")
        log_text.delete(0.0,"end")       
        log_text.config(state="disable")        
        temp = log_source_file.get(0.0,"end")
        files = temp.split("\n")
        print(len(files))
        print(files)
        '''
        sourceFolder="C:/Users/Mario/Desktop/Tianma 1.43''/ddd"
        pcPath="//lab01/D/t/"
        if not os.path.exists(pcPath):
            print("create pcPath")
            os.makedirs(pcPath)
        if os.path.exists(sourceFolder):
            print("delete pcPath")
            shutil.rmtree(pcPath)
        shutil.copytree(sourceFolder, pcPath)
        '''
        
  
btn_width=10
check_btn_width=7
###REFRESH
canvas_connect = tkinter.Canvas(top, borderwidth = 0, relief = 'raised')
canvas_connect.grid(row = 0,column = 0)

label_account=tkinter.Label(canvas_connect,text="Account:",disabledforeground='silver',width = btn_width)
label_account.grid(row=0,column=0,sticky="W")

entry_account = tkinter.Entry(canvas_connect,width = btn_width)
entry_account.grid(row = 0, column = 1,sticky="W")
entry_account.insert(0,"lab")

label_password=tkinter.Label(canvas_connect,text="Password:",disabledforeground='silver',width = btn_width)
label_password.grid(row=0,column=3,sticky="W")

entry_password = tkinter.Entry(canvas_connect,width = btn_width)
entry_password.grid(row = 0, column = 4,sticky="W")
entry_password.insert(0,"123456789")

btn_refresh= tkinter.Button(canvas_connect,text = "connect",width = btn_width,command = btn_CONNECT)
btn_refresh.grid(row = 0,column = 6,sticky="W")


###LAB
canvas_lab = tkinter.Canvas(top, borderwidth = 0, relief = 'raised')
canvas_lab.grid(row = 0,column = 1,columnspan = 2)

label_account=tkinter.Label(canvas_lab,text="PC status:",disabledforeground='silver',width = btn_width)
label_account.grid(row=0,column=0,sticky="W")

checkbtns = {}
checkbtn_names = ["lab01","lab02","lab03","lab04","lab05","lab06","lab07","lab08"]
for i in range(len(checkbtn_names)):
    checkbtns[checkbtn_names[i]]=tkinter.Checkbutton(canvas_lab,text = checkbtn_names[i],state="disable",disabledforeground='silver')
    checkbtns[checkbtn_names[i]].grid(row = 0,column = i+1)  

###TARGET
canvas_convert = tkinter.Canvas(top, borderwidth = 0, relief = 'raised')
canvas_convert.grid(row = 1,column = 0,columnspan = 3)

entry_TargetFile = tkinter.Entry(canvas_convert,width = 130)
entry_TargetFile.grid(row = 1, column = 0,columnspan = 9)

btn_openTarget= tkinter.Button(canvas_convert,text = "copy to",state="disable",disabledforeground='silver',width = btn_width,command = btn_OpenTargetFile)
btn_openTarget.grid(row = 1,column = 10)

###FUNCTION
canvas_function = tkinter.Canvas(top, borderwidth = 0, relief = 'raised')
canvas_function.grid(row = 2,column = 0)

btn_copy_file = tkinter.Button(canvas_function,text = "copy_file",state="disable",disabledforeground='silver',width = btn_width,command = btn_COPY_file)
btn_copy_file.grid(row = 0,column = 0)

btn_copy_folder = tkinter.Button(canvas_function,text = "copy_folder",state="disable",disabledforeground='silver',width = btn_width,command = btn_COPY_folder)
btn_copy_folder.grid(row = 0,column = 1)

btn_delete_file = tkinter.Button(canvas_function,text = "delete_file",state="disable",disabledforeground='silver',width = btn_width,command = btn_DELETE_file)
btn_delete_file.grid(row = 0,column = 2)

btn_delete_folder = tkinter.Button(canvas_function,text = "delete_folder",state="disable",disabledforeground='silver',width = btn_width,command = btn_DELETE_folder)
btn_delete_folder.grid(row = 0,column = 3)
     
###CLEAR
canvas_function = tkinter.Canvas(top, borderwidth = 0, relief = 'raised')
canvas_function.grid(row = 2,column = 2)

btn_clear = tkinter.Button(canvas_function,text = "clear",width = btn_width,command = btn_CLEAR)
btn_clear.grid(row = 0,column = 20)    
 
###SOURCE TEXT
canvas_source = tkinter.Canvas(top, borderwidth = 0, relief = 'raised')
canvas_source.grid(row = 3,column = 0)

log_source_file = tkinter.Text(canvas_source,width = 55,height = 15, state = "disable")
log_source_file.grid(row = 0,column = 0)

log_source_file_scrollbar = tkinter.Scrollbar(canvas_source)
log_source_file_scrollbar.grid(row = 0,column = 5,sticky = "n"+"s"+"e")
log_source_file.config(yscrollcommand = log_source_file_scrollbar.set)
log_source_file_scrollbar.config(command = log_source_file.yview)

###LOG TEXT
canvas_log = tkinter.Canvas(top, borderwidth = 0, relief = 'raised')
canvas_log.grid(row = 3,column =1 ,columnspan = 2)

log_text = tkinter.Text(canvas_log,width = 80, height = 15, state = "disable")
log_text.grid(row = 0,column = 0)

log_text_scrollbar = tkinter.Scrollbar(canvas_log)
log_text_scrollbar.grid(row = 0,column = 13,sticky = "n"+"s"+"e")
log_text.config(yscrollcommand = log_text_scrollbar.set)
log_text_scrollbar.config(command = log_text.yview)

btn_CONNECT()
top.mainloop()

       