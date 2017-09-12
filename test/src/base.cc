#include "../../lib/parser.h"
#include <iostream>

void test() {}

int main() {
  tkp::TokenType *t1 = new tkp::TokenType("number", sfsm::jsonNumber());
  tkp::TokenType *t2 = new tkp::TokenType("string", sfsm::jsonString());

  vector<tkp::TokenType *> types;
  types.push_back(t1);
  types.push_back(t2);

  tkp::Parser *parser = new tkp::Parser(types);
  parser->parse("123");

  return 0;
}
