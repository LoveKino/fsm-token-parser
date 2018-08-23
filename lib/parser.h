#pragma once

#include "../thirdparty/regulator/lib/parser.h"
#include "token.h"
#include "tokenType.h"

using namespace std;

namespace ftp {
class Parser {
  typedef pair<int, int> TokenState; // (tokenTypeIndex, state)
  typedef vector<TokenState> TokenStates;
  typedef pair<TokenStates, TokenStates> Configuration;

private:
  string fowardedText;
  vector<TokenType> tokenTypes;
  vector<Configuration> configurationRecords;

  void resetConfigurationRecords();
  void transitState(int tokenTypeIndex, int tokenState, char letter,
                    Configuration &newConfig);
  bool isEmptyConfiguration(Configuration &conf);
  bool fowardToEmptyConfiguration(string text);
  pair<int, int>
  findTheBestConfiguration(); // (configurationIndex, tokenTypeIndex)

  Token fetchToken();

public:
  Parser(vector<TokenType> &tokenTypes);

  vector<Token> parseChunk(string chunk);

  Configuration transit(Configuration &current, char letter);

  vector<Token> parseEnd();

  void displayRecords();
};

vector<Token> parseString(vector<TokenType> tokenTypes, string text);
} // namespace ftp
