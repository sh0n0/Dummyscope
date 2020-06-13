#include "lexer.hpp"

void Lexer::lexicalAnalysis() {}

std::shared_ptr<Token> Lexer::getCurToken() { return token_stream[cur_index]; }

std::shared_ptr<Token> Lexer::getNextToken() {
  return std::make_shared<Token>(Token(TOK_EOF, ""));
}

Token::Token(TokenType type, std::string str)
    : token_type(type), token_string(str) {
  if (type == TOK_NUMBER) {
    number = atoi(str.c_str());
  } else {
    number = std::numeric_limits<int>::max();
  }
}
