#include "ast.hpp"

void TranslationUnitAST::addFunction(std::unique_ptr<FunctionAST> func) {
  functions.push_back(std::move(func));
}

FunctionAST* TranslationUnitAST::getFunction(int idx) {
  if (idx >= functions.size()) return nullptr;
  return functions[idx].get();
}

std::string PrototypeAST::getArg(int idx) {
  if (idx >= args.size()) return nullptr;
  return args[idx];
}
ExprAST* CallExprAST::getArgExpr(int idx) {
  if (idx >= args.size()) return nullptr;
  return args[idx].get();
}
