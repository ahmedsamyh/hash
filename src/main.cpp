#define STDCPP_IMPLEMENTATION
#include <stdcpp.hpp>
#include <stack>

// static std::stack<Value> stack;

#define compiler_error(str, ...) compiler_error_impl(FMT(str, __VA_ARGS__))

void compiler_error_impl(const std::string& err_msg){
  fprint(std::cerr, "ERROR: {}\n", err_msg);
  exit(1);
}



struct Value {
  union Value_as {
    int Int;
    float Float;
    size_t Ptr;
    char Char;
  } as;
  
  enum Type {
    Int,
    Float,
    Str,
    Char,
    Ptr
  } type;

  Value(){};
  Value(const Type& _type){
    type = _type;
  }

  std::string type_as_str(){
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
      // res = FMT("\"{}\"", as.Str);
      UNIMPLEMENTED();
    } break;
    case Type::Char: {
      res = FMT("'{}'", as.Char);
    } break;
    case Type::Ptr: {
      res = FMT("{:x}", as.Ptr);
    } break;
    default: {
      UNREACHABLE();
    } break;
    }
    if (with_type){
      res += FMT("({})", type_as_str());
    }
    return res;
  }
};


struct Operator {
  enum Type{
    Sum,
    Sub,
    Mult,
    Div,
    Mod
  } type;

  std::string as_str(){
    switch (type){
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

};




// Expression is 2 operands and a operator that will evaluate to some value
// operand {optor} operand
// eg: 2 * 3;
struct Expression {
  Operator optor;
  Value a, b;

  Expression(){};

  Expression(const Operator::Type& optor_type, Value a, Value b){
    set_optor(optor_type);
    set_operands(a, b);
  };

  void set_optor(const Operator::Type& optor_type){
    optor.type = optor_type;
  }
  
  void set_operands(Value _a, Value _b){
    a = _a;
    b = _b;
  }

  std::string as_str(bool with_type=false){
    return FMT("{} {} {}", a.as_str(with_type), optor.as_str(), b.as_str(with_type));
  }

  void invalid_operands(){
    compiler_error("Invalid operands for `{}`: {}, {}", optor.as_str(), a.as_str(true), b.as_str(true));
  }

  void check_type_mismatch(){
    if (a.type != b.type){
      compiler_error("Type mismatch for operator `{}`: {}", optor.as_str(), as_str(true));
    }
  }

  Value eval(){
    // TODO: Check if sufficient operands are provided
    // compiler_error("Insufficient operands for `{}` operator\n", optor.as_str());
    
    Value res;
  
    check_type_mismatch();
    res.type = a.type;
    
    res.type = a.type;

    switch (optor.type){
    case Operator::Sum: {
      switch (a.type){
      case Value::Type::Int: {
	res.as.Int = a.as.Int + b.as.Int;
      } break;
      case Value::Type::Float: {
	res.as.Float = a.as.Float + b.as.Float;
      } break;
      case Value::Type::Str: {
	// concat
        // std::string c = FMT("{}{}", a.as.Str, b.as.Str);
	// res.as.Str = c.c_str();
	UNIMPLEMENTED();
      } break;
      case Value::Type::Char: {
	invalid_operands();
      } break;
      case Value::Type::Ptr: {
	res.as.Ptr = a.as.Ptr + b.as.Ptr;
      } break;
      default: {
	UNREACHABLE();
      } break;
      }
    } break;
    case Operator::Sub: {
      switch (a.type){
      case Value::Type::Int: {
	res.as.Int = a.as.Int - b.as.Int;
      } break;
      case Value::Type::Float: {
	res.as.Float = a.as.Float - b.as.Float;
      } break;
      case Value::Type::Str:
      case Value::Type::Char: {
	invalid_operands();
      } break;
      case Value::Type::Ptr: {
	res.as.Ptr = a.as.Ptr - b.as.Ptr;
      } break;
      default: {
	UNREACHABLE();
      } break;
      }
    } break;
    case Operator::Mult: {
      switch (a.type){
      case Value::Type::Int: {
	res.as.Int = a.as.Int * b.as.Int;
      } break;
      case Value::Type::Float: {
	res.as.Float = a.as.Float * b.as.Float;
      } break;
      case Value::Type::Str: {
	compiler_error("Cannot add two strings");
      } break;
      case Value::Type::Char: {
	compiler_error("Cannot add two characters");
      } break;
      case Value::Type::Ptr: {
	res.as.Ptr = a.as.Ptr * b.as.Ptr;
      } break;
      default: {
	UNREACHABLE();
      } break;
      }
    } break;
    case Operator::Div: {
      switch (a.type){
      case Value::Type::Int: {
	res.as.Int = a.as.Int / b.as.Int;
      } break;
      case Value::Type::Float: {
	res.as.Float = a.as.Float / b.as.Float;
      } break;
      case Value::Type::Str: {
	compiler_error("Cannot add two strings");
      } break;
      case Value::Type::Char: {
	compiler_error("Cannot add two characters");
      } break;
      case Value::Type::Ptr: {
	res.as.Ptr = a.as.Ptr / b.as.Ptr;
      } break;
      default: {
	UNREACHABLE();
      } break;
      }
    } break;
    case Operator::Mod: {
      switch (a.type){
      case Value::Type::Int: {
	res.as.Int = a.as.Int % b.as.Int;
      } break;
      case Value::Type::Float: {
	compiler_error("Cannot add two strings");
      } break;
      case Value::Type::Str: {
	compiler_error("Cannot add two strings");
      } break;
      case Value::Type::Char: {
	compiler_error("Cannot add two characters");
      } break;
      case Value::Type::Ptr: {
	compiler_error("Cannot add two strings");
      } break;
      default: {
	UNREACHABLE();
      } break;
      }
    } break;
    default: {
      UNREACHABLE();
    } break;
    }
    return res;
  }
};

struct Statement {
  std::vector<Expression> expressions;
};

int main(int argc, char *argv[]) {

  Value a(Value::Type::Ptr);
  a.as.Ptr = 3;

  Value b(Value::Type::Ptr);
  b.as.Ptr = 2;

  Expression expr1(Operator::Type::Sub, a, b);

  Value c = expr1.eval();

  Value d(Value::Type::Ptr);
  d.as.Ptr = 100;

  Expression expr2(Operator::Type::Sub, c, d);
  
  print("{} = {} = {}\n", expr1.as_str(), expr2.as_str(), expr2.eval().as_str());

  return 0;
}
