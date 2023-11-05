#define STDCPP_IMPLEMENTATION
#include <stdcpp.hpp>
#include <stack>

// static std::stack<Value> stack;

enum Type {
  Int,
  Float,
  Str,
  Char,
  Ptr
};

std::string type_as_name(const Type& type){
  switch (type){
  case Type::Int: {
    return "int";
  } break;
  case Type::Float: { 
    return "float";
  } break;
  case Type::Str: { 
    return "str";
  } break;
  case Type::Char: { 
    return "char";
  } break;
  case Type::Ptr: { 
    return "ptr";
  } break;
  default: {
    UNREACHABLE();
  } break;
  }
  return "Invalid Type";
}

union Value_as {
    int Int;
    float Float;
    size_t Ptr;
    char Char;
};

struct Value {
  Value_as as;
  Type type;

  std::string as_str(bool with_type=false){
    std::string res = "Invalid Value";
    switch(type){
    case Type::Int: {
      res = FMT("{}", as.Int);
    } break;
    case Type::Float: {
      res = FMT("{:.2f}f", as.Float);
    } break;
    case Type::Str: {
      UNIMPLEMENTED();
    } break;
    case Type::Char: {
      res = FMT("{}", as.Char);
    } break;
    case Type::Ptr: {
      res = FMT("{}", as.Ptr);
    } break;
    default: {
      UNREACHABLE();
    } break;
    }
    if (with_type){
      res += FMT("({})", type_as_name(type));
    }
    return res;
  }
};


enum Operator{
  Sum,
  Sub,
  Mult,
  Div,
  Mod
};

std::string optor_as_name(const Operator& optor){
  switch (optor){
  case Operator::Sum: {
    return "+";
} break;
  case Operator::Sub: {
    return "-";
} break;
  case Operator::Mult: {
    return "*";
} break;
  case Operator::Div: {
    return "/";
} break;
  case Operator::Mod  : {
    return "%";
} break;
  default: {
    UNREACHABLE();
  } break;
  }
  return "Invalid Operator";
}

// Expression is 2 operands and a operator that will evaluate to some value
// operand {optor} operand
// eg: 2 * 3;
struct Expression {
  Operator optor;
  std::vector<Value> operands;

  std::string as_str(){
    ASSERT(operands.size() == 2);
    return FMT("{} {} {}", operands[0].as_str(true), optor_as_name(optor), operands[1].as_str(true));
  }
};

struct Statement {
  std::vector<Expression> expressions;
};

#define compiler_error(str, ...) compiler_error_impl(FMT(str, __VA_ARGS__))

void compiler_error_impl(const std::string& err_msg){
  fprint(std::cerr, "ERROR: {}\n", err_msg);
  exit(1);
}

void check_type_mismatch(Expression& expr, const Value& a, const Value& b){
  if (a.type != b.type){
    compiler_error("Type mismatch for operator `{}`: {}", optor_as_name(expr.optor), expr.as_str());
  }
}

Value eval_expr(Expression& expr){
  Value result;
  if (expr.operands.size() < 2){
    compiler_error("Insufficient operands for `{}` operator\n", optor_as_name(expr.optor));
  }
  ASSERT(expr.operands.size() == 2);
  
  check_type_mismatch(expr, expr.operands[0], expr.operands[1]);
  result.type = expr.operands[0].type;

  
  switch (expr.optor){
  case Operator::Sum: {
    switch (expr.operands[0].type){
    case Type::Int: {
      result.as.Int = expr.operands[0].as.Int + expr.operands[1].as.Int;
    } break;
    case Type::Float: {
      result.as.Float = expr.operands[0].as.Float + expr.operands[1].as.Float;
    } break;
    case Type::Str: {
      compiler_error("Cannot add two strings");
    } break;
    case Type::Char: {
      compiler_error("Cannot add two characters");
    } break;
    case Type::Ptr: {
      result.as.Ptr = expr.operands[0].as.Ptr + expr.operands[1].as.Ptr;
    } break;
    default: {
      UNREACHABLE();
    } break;
    }
  } break;
  case Operator::Sub: {
    
  } break;
  case Operator::Mult: {
    
  } break;
  case Operator::Div: {
    
  } break;
  case Operator::Mod: {
    
  } break;
  default: {
    UNREACHABLE();
  } break;
  }
  return result;
}


int main(int argc, char *argv[]) {

  Value a;
  a.type = Type::Float;
  a.as.Float = 34.f;
  Value b;
  b.type = Type::Int;
  b.as.Int = 35;
  Expression expr;
  expr.optor = Operator::Sum;
  expr.operands.push_back(a);
  expr.operands.push_back(b);

  print("{} {} {} = {}\n", a.as_str(), optor_as_name(expr.optor), b.as_str(), eval_expr(expr).as_str());

  return 0;
}
