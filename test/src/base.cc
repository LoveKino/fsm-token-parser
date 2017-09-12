#include "../../lib/parser.h"
#include <assert.h>
#include <iostream>

void test() {}

vector<tkp::TokenType *> jsonTokenTypes() {
  vector<tkp::TokenType *> types;

  types.push_back(new tkp::TokenType("number", sfsm::jsonNumber()));
  types.push_back(new tkp::TokenType("string", sfsm::jsonString()));
  types.push_back(new tkp::TokenType("{", sfsm::box("{")));
  types.push_back(new tkp::TokenType("}", sfsm::box("}")));
  types.push_back(new tkp::TokenType(",", sfsm::box(",")));
  types.push_back(new tkp::TokenType(":", sfsm::box(":")));
  types.push_back(new tkp::TokenType("[", sfsm::box("[")));
  types.push_back(new tkp::TokenType("]", sfsm::box("]")));

  return types;
}

void equalTokens(vector<tkp::Token *> tokens, string *strs) {
  int index = -1;
  for (vector<tkp::Token *>::iterator it = tokens.begin(); it != tokens.end();
       ++it) {
    string tokenName = (*it)->getName();
    string tokenText = (*it)->getText();

    cout << "tokenName: " << tokenName << "\ntokenText: " << tokenText << endl;

    index++;
    string name = strs[index];
    assert(tokenName.compare(name) == 0);

    index++;
    string text = strs[index];
    assert(tokenText.compare(text) == 0);
  }
}

int main() {
  string list1[] = {"number", "123"};
  equalTokens((new tkp::Parser(jsonTokenTypes()))->parse("123"), list1);

  string list2[] = {"string", "\"text\""};
  equalTokens((new tkp::Parser(jsonTokenTypes()))->parse("\"text\""), list2);

  string list3[] = {"[", "[", "number", "1",      ",", ",", "number",
                    "2", ",", ",",      "number", "3", "]", "]"};
  equalTokens((new tkp::Parser(jsonTokenTypes()))->parse("[1,2,3]"), list3);

  string list4[] = {"{", "{",      "string",  "\"a\"", ":",
                    ":", "string", "\"bcd\"", "}",     "}"};
  equalTokens((new tkp::Parser(jsonTokenTypes()))->parse("{\"a\":\"bcd\"}"),
              list4);

  return 0;
}
