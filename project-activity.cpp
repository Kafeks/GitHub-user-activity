// g++ project-activity.cpp \
// -I/opt/homebrew/include \
// -I/opt/homebrew/opt/curl/include \
// -L/opt/homebrew/opt/curl/lib \
// -lcurl \
// -o project_activity



#include <iostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <string>

using namespace std;
using json = nlohmann::json;


void nickname(string &username) {
    cout << "Write username: ";
    cin >> username;
} 

size_t callback(void* data, size_t size, size_t count, string* result)
{
    result->append((char*)data, size * count);

    return size * count;
}

int main() {
    json name;
    string username;
    nickname(username);


    CURL* curl = curl_easy_init();

    string url = "https://api.github.com/users/" + username + "/events";
    string result;

    curl_easy_setopt(
        curl,
        CURLOPT_URL,
        url.c_str()
    );

    curl_easy_setopt (
        curl,
        CURLOPT_WRITEFUNCTION,
        callback
    );
    curl_easy_setopt(
        curl,
        CURLOPT_WRITEDATA,
        &result
    );
    curl_easy_setopt(
        curl,
        CURLOPT_USERAGENT,
        "project-activity"
    );

    curl_easy_perform(curl);
    json events = json::parse(result);

    for(auto& event : events) {
        string type = event["type"];
        string repo = event["repo"]["name"];
        cout << type << "->" << repo << "\n";
        
        if (type == "PushEvent") {
            int size = event["payload"]["size"];
            
            cout << "Repository: " << repo << "\n";
            cout << "Commits: " << size << "\n";
        }
        
    }
    return 0;
}