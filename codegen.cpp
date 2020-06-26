#include "codegen.hpp"

#include <llvm/IR/Verifier.h>

#include <iostream>

CodeGen::CodeGen() {
  context = std::make_unique<llvm::LLVMContext>();
  module = std::make_unique<llvm::Module>("dummyscope", *context);
  builder = std::make_unique<llvm::IRBuilder<>>(*context);
}

bool CodeGen::generate(TranslationUnitAST* ast) {
  addBuiltinFunction();

  for (int i = 0;; ++i) {
    FunctionAST* func = ast->getFunction(i);
    if (!func) break;

    if (!generateFunction(func)) {
      std::cerr << "Invalid function" << std::endl;
      return false;
    }
  }
  return true;
}

llvm::Function* CodeGen::generateFunction(FunctionAST* ast) {
  auto func = generateProtoType(ast->getProto());
  builder->SetInsertPoint(llvm::BasicBlock::Create(*context, "entry", func));

  clearNamedValue();
  for (auto& arg : func->args()) {
    setNamedValue(arg.getName(), &arg);
  }
  auto value = generateExpr(ast->getBody());
  if (!value) {
    func->eraseFromParent();
    std::cerr << "Invalid function body" << std::endl;
    return nullptr;
  }
  builder->CreateRet(value);
  llvm::verifyFunction(*func);
  return func;
}

llvm::Function* CodeGen::generateProtoType(PrototypeAST* ast) {
  std::vector<llvm::Type*> params(ast->getArgsSize(), llvm::Type::getInt32Ty(*context));
  auto func_type = llvm::FunctionType::get(llvm::Type::getInt32Ty(*context), params, false);
  auto func = llvm::Function::Create(func_type, llvm::Function::ExternalLinkage, ast->getName(), module.get());

  int idx = 0;
  for (auto& arg : func->args()) {
    arg.setName(ast->getArg(idx));
    idx++;
  }
  return func;
}

llvm::Value* CodeGen::generateExpr(ExprAST* ast) {
  if (llvm::isa<BinaryExprAST>(ast)) {
    return generateBinaryExpr(llvm::dyn_cast<BinaryExprAST>(ast));
  } else if (llvm::isa<CallExprAST>(ast)) {
    return generateCallExpr(llvm::dyn_cast<CallExprAST>(ast));
  } else if (llvm::isa<NumberAST>(ast)) {
    return generateNumber(llvm::dyn_cast<NumberAST>(ast));
  } else if (llvm::isa<VariableAST>(ast)) {
    return generateVariable(llvm::dyn_cast<VariableAST>(ast));
  } else {
    std::cerr << "Unknown ExprAST" << std::endl;
    return nullptr;
  }
}

llvm::Value* CodeGen::generateBinaryExpr(BinaryExprAST* ast) {
  llvm::Value* LHS = generateExpr(ast->getLHS());
  llvm::Value* RHS = generateExpr(ast->getRHS());
  if (!LHS || !RHS) {
    std::cerr << "Invalid expression in binary operation" << std::endl;
    return nullptr;
  }

  switch (ast->getOp()) {
    case ADD:
      return builder->CreateAdd(LHS, RHS, "addtmp");
    case SUB:
      return builder->CreateSub(LHS, RHS, "subtmp");
    case MUL:
      return builder->CreateMul(LHS, RHS, "multmp");
    case DIV:
      return builder->CreateUDiv(LHS, RHS, "divtmp");
    case UNDEFINED:
      std::cerr << "failed generating binary operation code" << std::endl;
      return nullptr;
  }
}

llvm::Value* CodeGen::generateNumber(NumberAST* ast) {
  auto int_value = llvm::ConstantInt::get(llvm::IntegerType::getInt32Ty(*context), ast->getValue(), true);
  return llvm::cast<llvm::Value>(int_value);
}

llvm::Value* CodeGen::generateCallExpr(CallExprAST* ast) {
  llvm::Function* callee = module->getFunction(ast->getCalleeName());
  if (!callee) {
    std::cerr << "Unknown function referenced" << std::endl;
    return nullptr;
  }
  if (callee->arg_size() != ast->getArgsSize()) {
    std::cerr << "mismatch argument count" << std::endl;
    return nullptr;
  }
  std::vector<llvm::Value*> argsV;
  for (int i = 0; i < ast->getArgsSize(); ++i) {
    llvm::Value* argValue = generateExpr(ast->getArgExpr(i));
    if (!argValue) {
      std::cerr << "Invalid Expr in call";
      return nullptr;
    }
    argsV.push_back(argValue);
  }

  return builder->CreateCall(callee, argsV, "calltmp");
}

llvm::Value* CodeGen::generateVariable(VariableAST* ast) {
  llvm::Value* value = named_values[ast->getName()];
  if (!value) {
    std::cerr << "failed generating variable code" << std::endl;
    return nullptr;
  }
  return value;
}

void CodeGen::setNamedValue(const std::string& name, llvm::Value* value) { named_values[name] = value; }

void CodeGen::clearNamedValue() { named_values.clear(); }

void CodeGen::addBuiltinFunction() {
  // print
  auto int32Type = builder->getInt32Ty();
  std::vector<llvm::Type*> print_args(1, int32Type->getPointerTo());
  auto func_type = llvm::FunctionType::get(builder->getVoidTy(), print_args, false);
  module->getOrInsertFunction("print", func_type);
}
