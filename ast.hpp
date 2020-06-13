class ExprAST {
 private:
 public:
  ExprAST(){};
  ~ExprAST(){};
};

class NumberAST : public ExprAST {
 private:
 public:
  NumberAST(){};
  ~NumberAST(){};
};

class VariableAST : public ExprAST {
 private:
 public:
  VariableAST(){};
  ~VariableAST(){};
};

class BinaryExprAST : public ExprAST {
 private:
 public:
  BinaryExprAST(){};
  ~BinaryExprAST(){};
};

class CallExprAST : public ExprAST {
 private:
 public:
  CallExprAST(){};
  ~CallExprAST(){};
};

class PrototypeAST {
 private:
 public:
  PrototypeAST(){};
  ~PrototypeAST(){};
};

class FunctionAST {
 private:
 public:
  FunctionAST(){};
  ~FunctionAST(){};
};

class TranslationUnitAST {
 private:
 public:
  TranslationUnitAST(){};
  ~TranslationUnitAST(){};
};
