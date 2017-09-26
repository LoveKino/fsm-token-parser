#include "token.h"
#include <iostream>

using namespace std;

namespace ftp {
string Token::getName() const { return this->name; }
string Token::getText() const { return this->text; }

bool Token::operator==(const Token &other) {
  return (this->name == other.name) && (this->text == other.text);
}

bool Token::operator!=(const Token &other) { return !(*this == other); }
} // namespace ftp
