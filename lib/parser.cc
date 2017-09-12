#include "parser.h"

namespace tkp {
TokenType::TokenType(string name, sfsm::StateBox *stateBox, int priority) {
  this->name = name;
  this->stateBox = stateBox;
  this->priority = priority;
}

TokenType::TokenType(string name, sfsm::StateBox *stateBox) {
  this->name = name;
  this->stateBox = stateBox;
  this->priority = 1;
}

string TokenType::getName() { return this->name; }
int TokenType::getPriority() { return this->priority; }
sfsm::StateBox *TokenType::getStateBox() { return this->stateBox; }

Token::Token(string name, string text) {
  this->name = name;
  this->text = text;
}

string Token::getName() { return this->name; }

string Token::getText() { return this->text; }

Configuration::Configuration(vector<TokenType *> types) {
  this->types = types;

  Ints parts;
  Ints matchs;

  vector<TokenType *>::iterator it;
  int index = 0;
  for (it = types.begin(); it != types.end(); ++it) {
    this->fsms.push_back(sfsm::fsm((*it)->getStateBox()));
    parts.push_back(index);
    index++;
  }

  this->partsMatchs = make_pair(parts, matchs);
}

IntsPair Configuration::getPartsMatchs() { return this->partsMatchs; };

TokenType *Configuration::getTokenType(int index) { return this->types[index]; }

void Configuration::transform(char letter) {
  vector<int> nextParts;
  vector<int> nextMatchs;

  Ints parts = this->partsMatchs.first;
  Ints matchs = this->partsMatchs.second;

  // parts
  for (Ints::iterator it = parts.begin(); it != parts.end(); ++it) {
    this->transitFSM(*it, letter, &nextMatchs, &nextParts);
  }

  // matchs
  for (Ints::iterator j = matchs.begin(); j != matchs.end(); ++j) {
    this->transitFSM(*j, letter, &nextMatchs, &nextParts);
  }

  this->partsMatchs = make_pair(nextParts, nextMatchs);
};

void Configuration::transitFSM(int index, char letter, Ints *nextMatchs,
                               Ints *nextParts) {
  sfsm::FSM *fsm = this->fsms[index];
  sfsm::TRANSITION_RESULT_TYPES type = fsm->transit(letter);
  if (type == sfsm::MATCH) {
    nextMatchs->push_back(index);
  } else if (type == sfsm::WAIT) {
    nextParts->push_back(index);
  }
}

Parser::Parser(vector<TokenType *> tokenTypes) {
  this->tokenTypes = tokenTypes;
  this->stock = "";
}

Token *Parser::getToken() {
  string::iterator it;
  vector<IntsPair> matrix;
  Configuration *configuration = new Configuration(this->tokenTypes);

  string prefix = "";

  for (it = this->stock.begin(); it != this->stock.end(); ++it) {
    char ch = (*it);
    prefix += string(1, ch);

    configuration->transform(ch);
    // meet the empty situation, no parts or matchs for current prefix,
    // it means even get more chunks, will still no parts or matchs.
    if (!configuration->getPartsMatchs().first.size() &&
        !configuration->getPartsMatchs().second.size()) {
      pair<int, int> matchRet = findTokenTypeFromMatrix(matrix, configuration);

      if (matchRet.first == -1) {
        throw new runtime_error("can not match prefix " + prefix);
      } else {
        Token *token =
            new Token(configuration->getTokenType(matchRet.first)->getName(),
                      prefix.substr(0, matchRet.second + 1));

        this->stock = this->stock.substr(
            matchRet.second + 1, this->stock.size() - matchRet.second - 1);

        return token;
      }
    } else {
      matrix.push_back(configuration->getPartsMatchs());
    }
  }

  return NULL;
}

pair<int, int> Parser::findTokenTypeFromMatrix(vector<IntsPair> matrix,
                                               Configuration *configuration) {
  TokenType *finded = NULL;
  int tokenTypeIndex = -1;

  int prefixIndex = -1;
  int index = -1;

  for (vector<IntsPair>::iterator it = matrix.begin(); it != matrix.end();
       ++it) {
    index++;

    Ints matchs = (*it).second;
    for (Ints::iterator j = matchs.begin(); j != matchs.end(); ++j) {
      TokenType *curTokenType = configuration->getTokenType(*j);
      if (finded == NULL ||
          finded->getPriority() <
              curTokenType->getPriority() || // consider priority
          finded->getPriority() ==
              curTokenType->getPriority() // later matching has a longer prefix,
                                          // choose the longest prefix one
      ) {
        finded = curTokenType;
        tokenTypeIndex = *j;
        prefixIndex = index;
      }
    }
  }

  return make_pair(tokenTypeIndex, prefixIndex);
}

vector<Token *> Parser::parseChunk(string chunk) {
  vector<Token *> tokens;
  this->stock += chunk; // update current stock

  while (this->stock.size()) {
    Token *token = getToken();
    if (token == NULL) {
      break;
    } else {
      tokens.push_back(token);
    }
  }

  return tokens;
}

vector<Token *> Parser::parseEnd() {
  vector<Token *> tokens;
  return tokens;
}

vector<Token *> Parser::parse(string data) {
  vector<Token *> tokens = this->parseChunk(data);
  vector<Token *> lastTokens = this->parseEnd();
  tokens.insert(tokens.end(), lastTokens.begin(), lastTokens.end());
  return tokens;
}
} // namespace tkp
