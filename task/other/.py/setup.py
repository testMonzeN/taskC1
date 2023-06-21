import os 
import platform
import getpass
import subprocess

wlanlist = ["wlan10", "wlan7", "wlanXP"]
def start(name):
    user = getpass.getuser()
    get_file = os.path("{name}.bat")
    filepath = get_file
    command = f'runas /user:{user} "{filepath}"'
    subprocess.run(command, shell=True)
    

    system = platform.release()
    if system == "10":
        start(wlanlist[0])
    elif system == "7":
        start(wlanlist[1])
    elif system == "XP":
        start(wlanlist[2])