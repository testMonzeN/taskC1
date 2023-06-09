import platform
import getpass
import subprocess
from wifi import main


wlanlist = ["wlan10", "wlan7", "wlanXP"]

def start(name):
    user = getpass.getuser()
    filepath = r'C:/Users/'+ user + '/Desktop/wifi/.bat/' + name + '.bat'
    command = f'runas /user:{user} "{filepath}"'
    subprocess.run(command, shell=True)

system = platform.release()

if system == "10":
    start(wlanlist[0])
elif system == "7":
    start(wlanlist[1])
elif system == "XP":
    start(wlanlist[2])

    
main()