#include "parser.hpp"

Parser::Parser(std::string filename) {
  lexer = std::make_unique<Lexer>(filename);
  lexer->lexicalAnalysis();
  curToken = lexer->getCurToken();
  peekToken = lexer->getNextToken();
}

void Parser::nextToken() {
  curToken = peekToken;
  peekToken = lexer->getNextToken();
}

std::shared_ptr<Token> Parser::getCurToken() { return curToken; }

std::shared_ptr<Token> Parser::getPeekToken() { return peekToken; }