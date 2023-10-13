#pragma once

#include <string>
#include <unordered_map>

template <class K, class V>
using hashmap = std::unordered_map<K, V>;

using std::string;

class Consts {
   public:
    struct Value {
        float f;
        int i;
    };

    static void Load();

    static int& GetInt(const string& key);
    static float& GetFloat(const string& key);

   private:
    static hashmap<string, Value> values;
    static Value& Get(const string& key);
};
