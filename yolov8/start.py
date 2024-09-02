#!/usr/bin/python3
import os
import time
import subprocess
import psutil 
def is_process_running(process_name_or_pid):  
    for proc in psutil.process_iter():  
        try:  
            if process_name_or_pid == proc.name() or str(process_name_or_pid) == str(proc.pid):  
                return True  
        except (psutil.NoSuchProcess, psutil.AccessDenied, psutil.ZombieProcess):  
            pass  
    return False  
    
def start_v2():  

    try:  
        os.chdir('/dev')
        os.popen('sudo chmod +666 myttyUSB') 
        os.chdir('/home/nf/Downloads/3/final/workspace') 
        os.system('./camera')  
        print("进程camera已启动")  
    except OSError as e:  
        print(f"启动camera程序时出错: {e}")  
    
    


if __name__ == "__main__":
    os.chdir('/dev')
    os.popen('sudo chmod +666 myttyUSB') 
    os.chdir('/home/nf/Downloads/3/final/workspace') 

    while True:  
        if not is_process_running('camera'):  
            start_v2()  
        time.sleep(3)  
