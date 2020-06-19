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
  TokenType mapStringToTokenType(const std::string& str);

 public:
  Lexer(std::string filename) : input_filename(filename){};
  ~Lexer(){};
  void lexicalAnalysis();
  std::unique_ptr<Token> getCurToken();
  int getCurIndex() { return cur_index; }
  std::unique_ptr<Token> getNextToken();
};
