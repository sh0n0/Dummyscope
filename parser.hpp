#include <memory>
#include <string>

#include "ast.hpp"
#include "lexer.hpp"

class Parser {
 private:
  std::shared_ptr<Token> curToken;
  std::shared_ptr<Token> peekToken;
  std::unique_ptr<Lexer> lexer;
  std::unique_ptr<TranslationUnitAST> TU;

  void nextToken();
  std::shared_ptr<Token> getCurToken();
  std::shared_ptr<Token> getPeekToken();

 public:
  Parser(std::string filename);
  ~Parser(){};
  bool parse();
  bool parseTransitionUnit();
  std::unique_ptr<FunctionAST> parseFunction();
  std::unique_ptr<PrototypeAST> parsePrototype();
  std::unique_ptr<FunctionAST> parseFunctionDefinition();
  std::unique_ptr<ExprAST> parseExpression();
};
