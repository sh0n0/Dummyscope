#include <memory>
#include <string>
#include <vector>

class ExprAST {
 private:
 public:
  ExprAST(){};
  ~ExprAST(){};
};

class NumberAST : public ExprAST {
 private:
  double value;

 public:
  NumberAST(double value) : value(value){};
  ~NumberAST(){};
  double getValue() { return value; }
};

class VariableAST : public ExprAST {
 private:
  std::string name;

 public:
  VariableAST(std::string name) : name(name){};
  ~VariableAST(){};
  std::string getName() { return name; }
};

class BinaryExprAST : public ExprAST {
 private:
  std::string op;
  std::unique_ptr<ExprAST> LHS, RHS;

 public:
  BinaryExprAST(std::string op, std::unique_ptr<ExprAST> LHS,
                std::unique_ptr<ExprAST> RHS)
      : op(op), LHS(std::move(LHS)), RHS(std::move(RHS)){};
  ~BinaryExprAST(){};
};

class CallExprAST : public ExprAST {
 private:
  std::string callee;
  std::vector<std::unique_ptr<ExprAST>> args;

 public:
  CallExprAST(std::string callee, std::vector<std::unique_ptr<ExprAST>> args)
      : callee(callee), args(std::move(args)){};
  ~CallExprAST(){};
};

class PrototypeAST {
 private:
  std::string name;
  std::vector<std::string> args;

 public:
  PrototypeAST(std::string name, std::vector<std::string> args)
      : name(name), args(args){};
  ~PrototypeAST(){};
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
};

class TranslationUnitAST {
 private:
  std::vector<std::unique_ptr<FunctionAST>> functions;

 public:
  TranslationUnitAST(){};
  ~TranslationUnitAST(){};
  void addFunction(std::unique_ptr<FunctionAST> func);
};
