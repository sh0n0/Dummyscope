#include "parser.hpp"

Parser::Parser(std::string filename) {
  lexer = std::make_unique<Lexer>(filename);
  lexer->lexicalAnalysis();
  curToken = lexer->getCurToken();
  peekToken = lexer->getNextToken();
  TU = std::make_unique<TranslationUnitAST>();
}

void Parser::nextToken() {
  curToken = peekToken;
  peekToken = lexer->getNextToken();
}

bool Parser::parse() { return parseTransitionUnit(); }

bool Parser::parseTransitionUnit() {
  while (curToken->getTokenType() != TOK_EOF) {
    auto func = parseFunctionDefinition();
    if (func) {
      TU->addFunction(std::move(func));
    } else {
      return false;
    }
  }
  return true;
}

std::unique_ptr<ExprAST> Parser::parseBinaryOpExpr(
    Precedence prev_prec, std::unique_ptr<ExprAST> LHS) {
  if (curToken->getTokenType() == TOK_EOF) return LHS;

  Precedence cur_prec = curPrecedence();
  if (cur_prec <= prev_prec) return LHS;

  OpType op = getOpType(curToken->getTokenType());
  if (op == UNDEFINED) return nullptr;
  nextToken();

  auto RHS = parsePrimary();
  if (!RHS) return nullptr;

  RHS = parseBinaryOpExpr(cur_prec, std::move(RHS));
  if (!RHS) return nullptr;

  return std::make_unique<BinaryExprAST>(op, std::move(LHS), std::move(RHS));
}

std::unique_ptr<ExprAST> Parser::parseExpression() {
  auto LHS = parsePrimary();
  if (!LHS) return nullptr;

  return parseBinaryOpExpr(LOWEST, std::move(LHS));
}

std::unique_ptr<ExprAST> Parser::parsePrimary() {
  switch (curToken->getTokenType()) {
    case TOK_IDENTIFIER:
      return parseIdentifierExpr();

    case TOK_NUMBER:
      return parseNumberExpr();

    case TOK_LPAREN:
      return parseParenExpr();

    default:
      return nullptr;
  }
}

std::unique_ptr<ExprAST> Parser::parseNumberExpr() {
  double num = curToken->getTokenNum();
  nextToken();
  return std::make_unique<NumberAST>(num);
}

std::unique_ptr<ExprAST> Parser::parseIdentifierExpr() {
  std::string name = curToken->getTokenString();

  nextToken();

  if (curToken->getTokenType() != TOK_LPAREN)
    return std::make_unique<VariableAST>(name);

  // Call
  auto call = parseCallExpr(name);
  if (!call) return nullptr;

  return call;
}

std::unique_ptr<ExprAST> Parser::parseParenExpr() {
  // eat (
  nextToken();

  auto expr = parseExpression();

  if (curToken->getTokenType() != TOK_RPAREN) return nullptr;

  // eat )
  nextToken();

  return expr;
}

std::unique_ptr<CallExprAST> Parser::parseCallExpr(
    const std::string& callee_name) {
  if (curToken->getTokenType() != TOK_LPAREN) return nullptr;

  // eat (
  nextToken();

  std::vector<std::unique_ptr<ExprAST>> args;
  auto arg = parseExpression();

  while (arg) {
    args.push_back(std::move(arg));

    if (curToken->getTokenType() == TOK_RPAREN) break;

    if (curToken->getTokenType() != TOK_COMMA) return nullptr;

    // eat ,
    nextToken();
    arg = parseExpression();
  }

  // eat )
  nextToken();

  return std::make_unique<CallExprAST>(callee_name, std::move(args));
}

std::unique_ptr<PrototypeAST> Parser::parsePrototype() {
  if (curToken->getTokenType() != TOK_IDENTIFIER) return nullptr;
  std::string fn_name = curToken->getTokenString();

  nextToken();

  if (curToken->getTokenType() != TOK_LPAREN) return nullptr;

  // eat (
  nextToken();

  std::vector<std::string> args;
  while (curToken->getTokenType() == TOK_IDENTIFIER) {
    args.push_back(curToken->getTokenString());
    nextToken();
  }

  if (curToken->getTokenType() != TOK_RPAREN) return nullptr;

  // eat )
  nextToken();

  return std::make_unique<PrototypeAST>(fn_name, std::move(args));
}

std::unique_ptr<FunctionAST> Parser::parseFunctionDefinition() {
  if (curToken->getTokenType() != TOK_DEF) return nullptr;

  // eat def
  nextToken();

  auto proto = parsePrototype();
  if (!proto) return nullptr;

  auto body = parseExpression();
  if (!body) return nullptr;

  return std::make_unique<FunctionAST>(std::move(proto), std::move(body));
}

Precedence Parser::curPrecedence() {
  return getPrecedence(curToken->getTokenType());
}
