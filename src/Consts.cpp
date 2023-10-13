#include "Consts.h"

#include <fstream>
#include <string>

#include "util.h"

#define MODULE "Consts"

hashmap<string, Consts::Value> Consts::values;

void Consts::Load() {
    std::fstream reader("consts.ini");

    string token, section;
    while (reader >> token) {
        if (token[0] == '[') {
            token.erase(token.begin());  // pop [
            token.pop_back();            // pop ]
            section = token;
            continue;
        }

        string equals, value;
        reader >> equals >> value;

        string key = section == "" ? token : (section + "." + token);
        Consts::values[key] = Consts::Value{
            std::stof(value),
            std::stoi(value),
        };
    }
}

Consts::Value& Consts::Get(const string& key) {
    auto item = Consts::values.find(key);
    if (item == Consts::values.end()) {
        fail2("key %s not found!", key.c_str());
    }

    return item->second;
}

int& Consts::GetInt(const string& key) { return Consts::Get(key).i; }
float& Consts::GetFloat(const string& key) { return Consts::Get(key).f; }
