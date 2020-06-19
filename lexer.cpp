#include "lexer.hpp"

#include <fstream>
#include <iostream>

void Lexer::lexicalAnalysis() {
  std::ifstream ifs(input_filename);
  if (!ifs) {
    exit(1);
  }

  std::string cur_line;
  while (getline(ifs, cur_line)) {
    std::cout << cur_line << std::endl;

    int idx = 0;
    while (idx < cur_line.length()) {
      auto next_token = read(cur_line, idx);
      token_stream.push_back(std::move(next_token));
    }
  }
  token_stream.push_back(std::make_unique<Token>(TOK_EOF, ""));
}

std::unique_ptr<Token> Lexer::read(const std::string& cur_line, int& idx) {
  std::string token_str;
  char next_char;

  // skip whitespace
  next_char = cur_line.at(idx);
  while (isspace(next_char)) {
    idx++;
    next_char = cur_line.at(idx);
  }

  // EOF
  if (next_char == EOF) return std::make_unique<Token>(TOK_EOF, token_str);

  // identifier
  if (isalpha(next_char)) {
    token_str += next_char;
    idx++;
    if (idx == cur_line.length()) {
      return std::make_unique<Token>(TOK_IDENTIFIER, token_str);
    }

    next_char = cur_line.at(idx);
    while (isalnum(next_char)) {
      token_str += next_char;
      idx++;
      if (idx == cur_line.length()) break;

      next_char = cur_line.at(idx);
    }

    if (token_str == "def") {
      return std::make_unique<Token>(TOK_DEF, token_str);
    } else {
      return std::make_unique<Token>(TOK_IDENTIFIER, token_str);
    }
  }

  // number
  if (isdigit(next_char)) {
    token_str += next_char;
    if (next_char == '0') return std::make_unique<Token>(TOK_NUMBER, token_str);
    idx++;
    if (idx == cur_line.length()) {
      return std::make_unique<Token>(TOK_NUMBER, token_str);
    }

    next_char = cur_line.at(idx);
    while (isdigit(next_char)) {
      token_str += next_char;
      idx++;
      if (idx == cur_line.length()) break;
      next_char = cur_line.at(idx);
    }
    return std::make_unique<Token>(TOK_NUMBER, token_str);
  }

  // Symbol
  token_str += next_char;
  idx++;
  return std::make_unique<Token>(mapStringToTokenType(token_str), token_str);
}

TokenType Lexer::mapStringToTokenType(const std::string& str) {
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

std::unique_ptr<Token> Lexer::getCurToken() {
  return std::move(token_stream[cur_index]);
}

std::unique_ptr<Token> Lexer::getNextToken() {
  if (cur_index == token_stream.size() - 1) {
    return nullptr;
  } else {
    cur_index++;
    return getCurToken();
  }
}

Token::Token(TokenType type, std::string str)
    : token_type(type), token_string(str) {
  if (type == TOK_NUMBER) {
    number = atoi(str.c_str());
  } else {
    number = std::numeric_limits<int>::max();
  }
}
