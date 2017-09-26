#include "parser.h"

using namespace std;

namespace ftp {
Parser::Parser(vector<TokenType> &tokenTypes) {
  this->tokenTypes = tokenTypes;
  this->resetConfigurationRecords();
}

void Parser::resetConfigurationRecords() {
  this->fowardedText = "";
  this->configurationRecords.clear();

  Configuration conf;
  unsigned int index = 0;
  for (auto i = tokenTypes.begin(); i != tokenTypes.end(); ++i) {
    conf.first.push_back({index, i->getRegExp().getStartState()});
    index++;
  }

  this->configurationRecords.push_back(conf);
}

bool Parser::isEmptyConfiguration(Configuration &conf) {
  return !conf.first.size() && !conf.second.size();
}

vector<Token> Parser::parseChunk(string chunk) {
  vector<Token> tokens;
  string stock = chunk;

  while (this->fowardToEmptyConfiguration(stock)) {
    Token token = this->fetchToken();
    tokens.push_back(token);
    int tokenTextSize = token.getText().size();

    // rest
    stock = stock.substr(tokenTextSize, stock.size() - tokenTextSize);
    this->resetConfigurationRecords();
  }

  return tokens;
}

Token Parser::fetchToken() {
  auto finded = this->findTheBestConfiguration();
  int configurationIndex = finded.first;
  int tokenTypeIndex = finded.second;

  if (configurationIndex == -1) {
    this->displayRecords();
    // TODO
    throw runtime_error("Can not parse text to tokens. Forwarded text is " +
                        this->fowardedText);
  } else {
    string tokenText = this->fowardedText.substr(0, configurationIndex);
    string tokenName = this->tokenTypes[tokenTypeIndex].getName();
    this->resetConfigurationRecords();

    return {tokenName, tokenText};
  }
}

void Parser::transitState(int tokenTypeIndex, int tokenState, char letter,
                          Configuration &newConfig) {
  auto regExp = this->tokenTypes[tokenTypeIndex].getRegExp();

  int nextState = regExp.transit(tokenState, letter);

  if (regExp.isEndState(nextState)) {
    // match
    newConfig.second.push_back({tokenTypeIndex, nextState});
  } else if (nextState != -1) {
    // part
    newConfig.first.push_back({tokenTypeIndex, nextState});
  }
}

Parser::Configuration Parser::transit(Configuration &current, char letter) {
  Configuration newConfig;

  //
  for (auto i = current.first.begin(); i != current.first.end(); ++i) {
    int tokenTypeIndex = i->first;
    int tokenState = i->second;
    this->transitState(tokenTypeIndex, tokenState, letter, newConfig);
  }

  //
  for (auto i = current.second.begin(); i != current.second.end(); ++i) {
    int tokenTypeIndex = i->first;
    int tokenState = i->second;

    this->transitState(tokenTypeIndex, tokenState, letter, newConfig);
  }

  return newConfig;
}

bool Parser::fowardToEmptyConfiguration(string text) {
  auto current = this->configurationRecords.back();
  for (auto i = text.begin(); i != text.end(); ++i) {
    char letter = *i;
    auto nextConf = this->transit(current, letter);

    if (this->isEmptyConfiguration(nextConf)) {
      // do not consume current letter, because it causes the empty situation
      // meet empty, prepare to fetch token
      return true;
    } else {
      // change current configuration
      this->configurationRecords.push_back(nextConf);
      this->fowardedText += string(1, letter);
      current = nextConf;
    }
  }

  return false;
}

/**
 * 1. biggest prority
 * 2. longest for same token type
 */
pair<int, int> Parser::findTheBestConfiguration() {
  int configurationIndex = -1;
  int tokenTypeIndex = -1;
  int priority = -1;

  int recordSize = this->configurationRecords.size();
  for (int i = 0; i < recordSize; i++) {
    auto matchs = this->configurationRecords[i].second;
    int matchSize = matchs.size();
    for (int j = 0; j < matchSize; j++) {
      int ttIndex = matchs[j].first;
      if (configurationIndex == -1) {
        configurationIndex = i;
        // token type index
        tokenTypeIndex = ttIndex;
        priority = this->tokenTypes[ttIndex].getPriority();
      } else {
        if (tokenTypeIndex == ttIndex) {
          configurationIndex = i; // choose the longest
        } else if (this->tokenTypes[tokenTypeIndex].getPriority() <
                   this->tokenTypes[ttIndex]
                       .getPriority()) { // choose the higher priority
          configurationIndex = i;
          // token type index
          tokenTypeIndex = ttIndex;
          priority = this->tokenTypes[ttIndex].getPriority();
        }
      }
    }
  }

  return {configurationIndex, tokenTypeIndex};
}

vector<Token> Parser::parseEnd() {
  vector<Token> tokens;
  string rest = this->fowardedText;

  while (rest.size()) {
    Token token = this->fetchToken();
    tokens.push_back(token);
    int tokenTextSize = token.getText().size();
    rest = rest.substr(tokenTextSize, rest.size() - tokenTextSize);
  }

  return tokens;
}

void Parser::displayRecords() {
  cout << "configuration record: " << endl;
  for (auto i = this->configurationRecords.begin();
       i != this->configurationRecords.end(); ++i) {
    auto parts = i->first;
    auto matchs = i->second;

    cout << "   part: ";
    for (auto j = parts.begin(); j != parts.end(); ++j) {
      cout << "(" << j->first << ", " << j->second << ")  ";
    }

    cout << endl << "   match: ";
    for (auto k = matchs.begin(); k != parts.end(); ++k) {
      cout << "(" << k->first << ", " << k->second << ")  ";
    }
  }

  cout << endl;
}

vector<Token> parseString(vector<TokenType> tokenTypes, string text) {
  Parser parser(tokenTypes);
  vector<Token> tokens = parser.parseChunk(text);
  vector<Token> lastTokens = parser.parseEnd();

  tokens.insert(tokens.end(), lastTokens.begin(), lastTokens.end());
  return tokens;
}

} // namespace ftp
