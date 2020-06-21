#pragma once
#include <memory>
#include <string>
#include <vector>

enum TokenType {
  TOK_EOF,
  TOK_DEF,
  TOK_IDENTIFIER,
  TOK_NUMBER,
  TOK_LPAREN,
  TOK_RPAREN,
  TOK_PLUS,
  TOK_MINUS,
  TOK_ASTERISK,
  TOK_SLASH,
  TOK_COMMA,
};

enum OpType { ADD, SUB, MUL, DIV, UNDEFINED };

static TokenType mapStringToTokenType(const std::string& str) {
  if (str == "(") {
    return TOK_LPAREN;
  } else if (str == ")") {
    return TOK_RPAREN;
  } else if (str == "+") {
    return TOK_PLUS;
  } else if (str == "-") {
    return TOK_MINUS;
  } else if (str == "*") {
    return TOK_ASTERISK;
  } else if (str == "/") {
    return TOK_SLASH;
  } else if (str == ",") {
    return TOK_COMMA;
  }
}

static OpType getOpType(TokenType type) {
  switch (type) {
    case TOK_PLUS:
      return ADD;
    case TOK_MINUS:
      return SUB;
    case TOK_ASTERISK:
      return MUL;
    case TOK_SLASH:
      return DIV;
    default:
      return UNDEFINED;
  }
}

class Token {
 private:
  TokenType token_type;
  std::string token_string;
  int number;

 public:
  Token(TokenType type, std::string str);
  ~Token(){};
  TokenType getTokenType() { return token_type; };
  const std::string& getTokenString() { return token_string; };
  int getTokenNum() { return number; };
};

class Lexer {
 private:
  std::string input_filename;
  std::vector<std::unique_ptr<Token>> token_stream;
  int cur_index = 0;
  std::unique_ptr<Token> read(const std::string& cur_line, int& idx);

 public:
  Lexer(std::string filename) : input_filename(filename){};
  ~Lexer(){};
  void lexicalAnalysis();
  Token* getCurToken();
  int getCurIndex() { return cur_index; }
  Token* getNextToken();
};
