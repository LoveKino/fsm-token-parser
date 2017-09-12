#pragma once

#include "../thirdparty/simple-fsm-dsl/lib/fsm.h"
#include <iostream>

using namespace std;

namespace tkp {
class TokenType {
private:
  string name;
  sfsm::StateBox *stateBox;
  int priority;

public:
  TokenType(string name, sfsm::StateBox *stateBox);
  TokenType(string name, sfsm::StateBox *stateBox, int priority);

  string getName();
  int getPriority();
  sfsm::StateBox *getStateBox();
};

class Token {
private:
  string name;
  string text;

public:
  Token(string name, string text);
  string getName();
  string getText();
};

typedef vector<int> Ints;
typedef pair<Ints, Ints> IntsPair;

class Configuration {
private:
  vector<TokenType *> types;
  vector<sfsm::FSM *> fsms;
  IntsPair partsMatchs;

  void transitFSM(int index, char letter, Ints *nextMatchs, Ints *nextParts);

public:
  Configuration(vector<TokenType *> types);
  void transform(char letter);
  IntsPair getPartsMatchs();
  TokenType *getTokenType(int index);
};

class Parser {
private:
  vector<TokenType *> tokenTypes; // token types definition

  // inner states
  string stock; // store rest chars for chunk handling

  Token *getToken(bool end);
  pair<int, int> findTokenTypeFromMatrix(vector<IntsPair> matrix,
                                         Configuration *configuration);

public:
  Parser(vector<TokenType *> tokenTypes);

  vector<Token *> parse(string data);

  /**
   * parse data chunk by chunk until meeting the end.
   */
  vector<Token *> parseChunk(string chunk);

  vector<Token *> parseEnd();
};
} // namespace tkp
