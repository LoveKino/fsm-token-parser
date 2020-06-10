#include "../lib/parser.h"
#include "../deps/regulator/lib/commonTokenReg.h"
#include <assert.h>
#include <iostream>

using namespace std;
using namespace ftp;
using namespace sfsm;

namespace ftp_test {
void displayTokens(vector<Token> &tokens) {
  for (auto i = tokens.begin(); i != tokens.end(); ++i) {
    cout << "Token Name: " << i->getName() << "  token text: " << i->getText()
         << endl;
  }
}

int compareTokens(vector<Token> tokens1, vector<Token> tokens2) {
  if (tokens1.size() != tokens2.size())
    return -1;
  int index = 0;
  for (auto i = tokens1.begin(); i != tokens1.end(); i++) {
    Token token = *i;
    if (token != tokens2[index]) {
      return index;
    }
    index++;
  }

  return -1;
}

typedef vector<string> TokenTypeParams;
typedef vector<Token> Tokens;
typedef pair<string, Tokens> DataCase;

void testParseString(vector<TokenTypeParams> tokenTypeParams,
                     vector<DataCase> cases) {
  vector<TokenType> tokenTypes;
  for (auto i = tokenTypeParams.begin(); i != tokenTypeParams.end(); ++i) {
    auto item = *i;
    RegularExp regExp(item[1]);
    TokenType tokenType(item[0], regExp, stoi(item[2]));
    tokenTypes.push_back(tokenType);
  }

  for (auto i = cases.begin(); i != cases.end(); ++i) {
    string data = i->first;
    Tokens tokens = i->second;
    Tokens result = parseString(tokenTypes, data);

    cout << "[parse text]" << data << endl;

    int ret = compareTokens(tokens, result);
    if (ret != -1) {
      cout << "errored index is " << ret << endl;
      cout << "expect:";
      displayTokens(tokens);
      cout << "real:";
      displayTokens(result);
    }

    assert(ret == -1);
  }
}
};
