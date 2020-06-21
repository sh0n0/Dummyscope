#pragma once
#include <memory>
#include <string>

#include "ast.hpp"
#include "lexer.hpp"

enum Precedence { LOWEST, SUM, PRODUCT, CALL };

static Precedence getPrecedence(TokenType type) {
  switch (type) {
    case TOK_PLUS:
    case TOK_MINUS:
      return SUM;

    case TOK_ASTERISK:
    case TOK_SLASH:
      return PRODUCT;

    default:
      return LOWEST;
  }
}

class Parser {
 private:
  Token* curToken;
  Token* peekToken;
  std::unique_ptr<Lexer> lexer;
  std::unique_ptr<TranslationUnitAST> TU;

  void nextToken();
  Precedence curPrecedence();

 public:
  Parser(std::string filename);
  ~Parser(){};
  TranslationUnitAST* getTranslationUnitAST() { return TU.get(); }
  bool parse();

 private:
  bool parseTransitionUnit();
  std::unique_ptr<PrototypeAST> parsePrototype();
  std::unique_ptr<FunctionAST> parseFunctionDefinition();
  std::unique_ptr<ExprAST> parseExpression();
  std::unique_ptr<ExprAST> parsePrimary();
  std::unique_ptr<ExprAST> parseNumberExpr();
  std::unique_ptr<ExprAST> parseIdentifierExpr();
  std::unique_ptr<ExprAST> parseParenExpr();
  std::unique_ptr<CallExprAST> parseCallExpr(const std::string& callee_name);
  std::unique_ptr<ExprAST> parseBinaryOpExpr(Precedence prev_prec,
                                             std::unique_ptr<ExprAST> LHS);
};
