#include "tokenType.h"

using namespace std;

namespace ftp {
string TokenType::getName() const { return this->name; }

int TokenType::getPriority() const { return this->priority; }

sfsm::RegularExp TokenType::getRegExp() const { return this->regExp; }
} // namespace ftp
