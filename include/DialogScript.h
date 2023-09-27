#pragma once

#include <string>
#include <vector>
using std::string;
using std::vector;

struct DialogLine {
    enum Side { Left, Right };
    Side side;
    string character;
    string content;
};

struct DialogScript : public vector<DialogLine> {
    static DialogScript fromFile(const string& file);
};
