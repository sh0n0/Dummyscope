#include <memory>
#include <string>

#include "lexer.hpp"

class Parser {
 private:
  std::shared_ptr<Token> curToken;
  std::shared_ptr<Token> peekToken;
  std::unique_ptr<Lexer> lexer;

 public:
  Parser(std::string filename);
  ~Parser(){};
  void nextToken();
  std::shared_ptr<Token> getCurToken();
  std::shared_ptr<Token> getPeekToken();
};
