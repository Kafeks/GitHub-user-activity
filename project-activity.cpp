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

void nickname(string &username)
{
    cout << "Write username: ";
    cin >> username;
}

size_t callback(void *data, size_t size, size_t count, string *result)
{
    result->append((char *)data, size * count);

    return size * count;
}

int main()
{
    json name;
    string username;
    nickname(username);

    CURL *curl = curl_easy_init();

    string token;
    cout << "Enter your GitHub Token (or press Enter to skip): ";
    cin.ignore(); 
    getline(cin, token);

    struct curl_slist *headers = NULL;

    if (!token.empty())
    {
        string authHeader = "Authorization: Bearer " + token;
        headers = curl_slist_append(headers, authHeader.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    }

    string url = "https://api.github.com/users/" + username + "/events";
    string result;
    curl_easy_setopt(
        curl,
        CURLOPT_URL,
        url.c_str());

    curl_easy_setopt(
        curl,
        CURLOPT_WRITEFUNCTION,
        callback);
    curl_easy_setopt(
        curl,
        CURLOPT_WRITEDATA,
        &result);
    curl_easy_setopt(
        curl,
        CURLOPT_USERAGENT,
        "project-activity");

    curl_easy_perform(curl);
    json events = json::parse(result);

    if (!events.is_array())
    {
        cout << "Ошибка: " << events["message"] << "\n";
        return 1;
    }
    for (auto &event : events)
    {
        string type = event["type"];
        string repo = event["repo"]["name"];
        cout << type << "->" << repo << "\n";

        if (type == "PushEvent")
        {
            if (event["payload"].contains("before") && !event["payload"]["before"].is_null())
            {

                string before = event["payload"]["before"];
                string head = event["payload"]["head"];

                string compareUrl = "https://api.github.com/repos/" + repo + "/compare/" + before + "..." + head;
                string compareResult;

                curl_easy_setopt(curl, CURLOPT_URL, compareUrl.c_str());
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &compareResult);
                curl_easy_perform(curl);

                json compare = json::parse(compareResult);

                if (compare.contains("total_commits"))
                {

                    int compareSize = compare["total_commits"];

                    cout << "Commits: " << compareSize << "\n";
                }
            }
        }
    }

    return 0;
}