#pragma once

#include <iostream>

using namespace std;

namespace ftp {
class Token {
private:
  string name;
  string text;

public:
  Token(string n, string t) : name(n), text(t){};

  string getName() const;

  string getText() const;

  bool operator==(const Token &other);
  bool operator!=(const Token &other);
};
} // namespace ftp
