#include <memory>
#include <string>
#include <vector>

enum TokenType { TOK_EOF, TOK_DEF, TOK_EXTERN, TOK_IDENTIFIER, TOK_NUMBER };

class Token {
 private:
  TokenType token_type;
  std::string token_string;
  int number;

 public:
  Token(TokenType type, std::string str);
  ~Token(){};
};

class Lexer {
 private:
  std::string input_filename;
  std::vector<std::shared_ptr<Token>> token_stream;
  int cur_index;

 public:
  Lexer(std::string filename) : input_filename(filename){};
  ~Lexer(){};
  void lexicalAnalysis();
  std::shared_ptr<Token> getCurToken();
  int getCurIndex() { return cur_index; }
  std::shared_ptr<Token> getNextToken();
};
