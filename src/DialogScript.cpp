#include "DialogScript.h"

#include <cctype>
#include <fstream>
#include <string>

#include "util.h"

#define MODULE "DialogScript"

DialogScript DialogScript::fromFile(const string &file) {
  std::ifstream reader(file);
  if (!reader.is_open()) {
    fail2("fromFile couldn't open " YELLOW "%s" RESET "!", file.c_str());
  }

  DialogScript res;

  DialogLine line;
  char side;
  while (reader >> side >> line.character) {
    if (side != '>' && side != '<')
      fail2("failed to parse " YELLOW "%s" RESET "!\n"
            "The first character of every line should be '>' or '<'\n"
            "Here's an example of a valid line:\n" BLUE
            "< Copper: Hello World!" RESET,
            file.c_str());

    line.side = side == '<' ? DialogLine::Left : DialogLine::Right;

    std::getline(reader, line.content);
    while (line.content.back() == '\n' || line.content.back() == '\r')
      line.content.pop_back();

    while (line.character.back() == ':')
      line.character.pop_back();

    for (auto &chr : line.character) {
      chr = std::tolower(chr);
    }

    res.emplace_back(line);
  }

  return res;
}
