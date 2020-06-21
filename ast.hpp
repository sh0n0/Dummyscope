#pragma once
#include <memory>
#include <string>
#include <vector>

#include "lexer.hpp"

enum AstId { NumberExpr, VariableExpr, CallExpr, BinaryExpr };

class ExprAST {
 private:
  AstId id;

 public:
  ExprAST(AstId id) : id(id){};

  ~ExprAST(){};

  AstId getValueId() const { return id; }
};

class NumberAST : public ExprAST {
 private:
  int value;

 public:
  NumberAST(double value) : ExprAST(NumberExpr), value(value){};

  ~NumberAST(){};

  int getValue() { return value; }

  static inline bool classof(ExprAST const* expr) {
    return expr->getValueId() == NumberExpr;
  }
};

class VariableAST : public ExprAST {
 private:
  std::string name;

 public:
  VariableAST(std::string name) : ExprAST(VariableExpr), name(name){};

  ~VariableAST(){};

  std::string getName() { return name; }

  static inline bool classof(ExprAST const* expr) {
    return expr->getValueId() == VariableExpr;
  }
};

class BinaryExprAST : public ExprAST {
 private:
  OpType op;
  std::unique_ptr<ExprAST> LHS, RHS;

 public:
  BinaryExprAST(OpType op, std::unique_ptr<ExprAST> LHS,
                std::unique_ptr<ExprAST> RHS)
      : ExprAST(BinaryExpr), op(op), LHS(std::move(LHS)), RHS(std::move(RHS)){};

  ~BinaryExprAST(){};
  ExprAST* getLHS() { return LHS.get(); }
  ExprAST* getRHS() { return RHS.get(); }
  OpType getOp() { return op; }

  static inline bool classof(ExprAST const* expr) {
    return expr->getValueId() == BinaryExpr;
  }
};

class CallExprAST : public ExprAST {
 private:
  std::string callee;
  std::vector<std::unique_ptr<ExprAST>> args;

 public:
  CallExprAST(std::string callee, std::vector<std::unique_ptr<ExprAST>> args)
      : ExprAST(CallExpr), callee(callee), args(std::move(args)){};

  ~CallExprAST(){};

  std::string getCalleeName() { return callee; }
  int getArgsSize() { return args.size(); }
  ExprAST* getArgExpr(int idx);

  static inline bool classof(ExprAST const* expr) {
    return expr->getValueId() == CallExpr;
  }
};

class PrototypeAST {
 private:
  std::string name;
  std::vector<std::string> args;

 public:
  PrototypeAST(std::string name, std::vector<std::string> args)
      : name(name), args(args){};

  ~PrototypeAST(){};

  std::string getName() { return name; }
  int getArgsSize() { return args.size(); }
  std::string getArg(int idx);
};

class FunctionAST {
 private:
  std::unique_ptr<PrototypeAST> proto;
  std::unique_ptr<ExprAST> body;

 public:
  FunctionAST(std::unique_ptr<PrototypeAST> proto,
              std::unique_ptr<ExprAST> body)
      : proto(std::move(proto)), body(std::move(body)){};

  ~FunctionAST(){};

  PrototypeAST* getProto() { return proto.get(); }

  ExprAST* getBody() { return body.get(); }
};

class TranslationUnitAST {
 private:
  std::vector<std::unique_ptr<FunctionAST>> functions;

 public:
  TranslationUnitAST(){};

  ~TranslationUnitAST(){};

  void addFunction(std::unique_ptr<FunctionAST> func);

  FunctionAST* getFunction(int idx);
};
