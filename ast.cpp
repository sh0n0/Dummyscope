#include "ast.hpp"

void TranslationUnitAST::addFunction(std::unique_ptr<FunctionAST> func) {
  functions.push_back(std::move(func));
}