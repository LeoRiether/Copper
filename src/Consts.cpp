#include "Consts.h"

#include <exception>
#include <fstream>
#include <string>

#include "SDL_include.h"
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
        reader >> equals;
        getline(reader, value);
        while (!value.empty() && value[0] == ' ') value.erase(value.begin());

        string key = section == "" ? token : (section + "." + token);

        Consts::Value x;
        try {
            x.f = std::stof(value);
        } catch (std::exception const&) {
        }
        try {
            x.i = std::stoi(value);
        } catch (std::exception const&) {
        }
        x.s = value;

        if (key != "debug.show_colliders" ||
            Consts::values.find("debug.show_colliders") ==
                Consts::values.end()) {
            Consts::values[key] = x;
        }
    }
}

void Consts::PeriodicLoad() {
    static int lastLoad = 0;
    int now = SDL_GetTicks();
    if (now - lastLoad > 1000) {
        lastLoad = now;
        Consts::Load();
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
string& Consts::GetString(const string& key) { return Consts::Get(key).s; }
