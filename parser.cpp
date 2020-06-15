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

bool Parser::parse() { return parseTransitionUnit(); }

bool Parser::parseTransitionUnit() {
  while (true) {
    auto func = parseFunctionDefinition();
    if (func) {
      TU->addFunction(std::move(func));
    }
    if (curToken->getTokenType() == TOK_EOF) break;

    nextToken();
  }
  return true;
}

std::unique_ptr<ExprAST> Parser::parseExpression() {}

std::unique_ptr<PrototypeAST> Parser::parsePrototype() {
  if (curToken->getTokenType() != TOK_IDENTIFIER) return nullptr;
  std::string fn_name = curToken->getTokenString();

  nextToken();

  if (curToken->getTokenString() != "(") return nullptr;

  nextToken();  // eat (

  std::vector<std::string> args;
  while (curToken->getTokenType() == TOK_IDENTIFIER) {
    args.push_back(curToken->getTokenString());
    nextToken();
  }

  if (curToken->getTokenString() != ")") return nullptr;

  nextToken();  // eat )

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
