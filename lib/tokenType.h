#pragma once

#include "regularExp.h"

using namespace std;

namespace ftp {
class TokenType {
private:
  string name;
  int priority;
  sfsm::RegularExp regExp;

public:
  TokenType(string n, sfsm::RegularExp &reg, int p) : name(n), regExp(reg), priority(p){};
  string getName() const;
  int getPriority() const;
  sfsm::RegularExp getRegExp() const;
};
} // namespace ftp
