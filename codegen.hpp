#pragma once
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>

#include <memory>

#include "ast.hpp"

class ExprAST;

class CodeGen {
 private:
  std::unique_ptr<llvm::LLVMContext> context;
  std::unique_ptr<llvm::Module> module;
  std::unique_ptr<llvm::IRBuilder<>> builder;
  std::map<std::string, llvm::Value*> named_values;

 public:
  CodeGen();

  ~CodeGen(){};

  bool generate(TranslationUnitAST* ast);
  llvm::Module* getModule() { return module.get(); }

 private:
  llvm::Function* generateFunction(FunctionAST* ast);

  llvm::Value* generateNumber(NumberAST* ast);

  llvm::Function* generateProtoType(PrototypeAST* ast);

  llvm::Value* generateExpr(ExprAST* ast);

  llvm::Value* generateBinaryExpr(BinaryExprAST* ast);

  llvm::Value* generateCallExpr(CallExprAST* ast);

  void setNamedValue(const std::string& name, llvm::Value* value);

  void clearNamedValue();
  llvm::Value* generateVariable(VariableAST* ast);
};
