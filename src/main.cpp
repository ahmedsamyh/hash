#define STDCPP_IMPLEMENTATION
#include <stdcpp.hpp>
#include <stack>
#include <filesystem>
namespace fs = std::filesystem;

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

struct Loc{
  int col{0}, row{0};
  std::string file_path;

  std::string as_str() const {
    return FMT("{}:{}:{}", file_path, row, col);
  }
};

struct Token{
  enum Type{
    Name,
    Number,
    Open_paren,
    Close_paren,
    Semi_colon,
    Comma,
    Minus,
    Plus,
    Mult,
    Div,
    Mod,
    Equal,
    Returner,
    Open_curl,
    Close_curl,
    D_quote,
    Quote,
    String,
    Char
  } type;
  std::string value;
  Loc loc;

  std::string type_as_str(){
    switch (type){
    case Type::Name: {
      return "Name";
    } break;
    case Type::Number: {
      return "Number";
    } break;
    case Type::Open_paren: {
      return "Open_paren";
    } break;
    case Type::Close_paren: {
      return "Close_paren";
    } break;
    case Type::Semi_colon: {
      return "Semi_colon";
    } break;
    case Type::Comma: {
      return "Comma";
    } break;
    case Type::Minus: {
      return "Minus";
    } break;
    case Type::Plus: {
      return "Plus";
    } break;
    case Type::Mult: {
      return "Mult";
    } break;
    case Type::Div: {
      return "Div";
    } break;
    case Type::Mod: {
      return "Mod";
    } break;
    case Type::Equal: {
      return "Equal";
    } break;
    case Type::Returner: {
      return "Returner";
    } break;
    case Type::Open_curl: {
      return "Open_curl";
    } break;
    case Type::Close_curl: {
      return "Close_curl";
    } break;
    case Type::D_quote: {
      return "D_quote";
    } break;
    case Type::Quote: {
      return "Quote";
    } break;
    case Type::String: {
      return "String";
    } break;
    case Type::Char: {
      return "Char";
    } break;
    default: {
      UNREACHABLE();
    } break;
    }
    return "Invalid Token Type";
  }
  
  std::string as_str(){
    return FMT("{{ value: \"{}\", type: {} }}", value, type_as_str());
  }
};

#define FILE_EXT "hash"

std::vector<Token> parse_source_file(const std::string& filename){
  std::string file_ext = str::rpop_until(filename, '.');
  if (file_ext != FILE_EXT){
    fprint(std::cerr, "ERROR: Hash source files must have the extension `{}`!\n", FILE_EXT);
    exit(1);
  }
  std::string file = file::slurp_file(filename);
  std::vector<Token> res;
  if (file.empty()){
    print("WARNING: File {} is empty\n", filename);
    return res;
  }

  file = str::trim(file);
  file = str::remove_char(file, '\r');
  int col = 1, row = 1;


  while (!file.empty()){
    
    std::string line = str::lpop_until(file, '\n');
    line += '\n';
    file = str::lremove(file, line.size());

    auto isnotalpha = [](const char& ch){
      return !ch::isalpha(ch);
    };
    auto isnotdigit = [](const char& ch){
      return !ch::isdigit(ch);
    };
    
    while (!line.empty()) {
      Token token;
      token.loc.file_path = fs::absolute(fs::path(filename)).string();
      token.loc.col = col;
      token.loc.row = row;
      if (ch::isalpha(line[0])) {
	token.value = str::lpop_until(line, isnotalpha);
	token.type = Token::Type::Name;
	line = str::lremove(line, token.value.size());
        res.push_back(token);
      } else if (ch::isdigit(line[0])){
	token.value = str::lpop_until(line, isnotdigit);
	token.type = Token::Type::Number;
	line = str::lremove(line, token.value.size());
	res.push_back(token);	
      } else if (line[0] == '(') {
	token.value = str::lpop(line, 1);
	token.type = Token::Type::Open_paren;
	line = str::lremove(line, 1);
	res.push_back(token);
      } else if (line[0] == ')') {
	token.value = str::lpop(line, 1);
	token.type = Token::Type::Close_paren;
	line = str::lremove(line, 1);
	res.push_back(token);
      } else if (line[0] == ','){
	token.value = str::lpop(line, 1);
	token.type = Token::Type::Comma;
	line = str::lremove(line, 1);
	res.push_back(token);
      } else if (line[0] == ';'){
	token.value = str::lpop(line, 1);
	token.type = Token::Type::Semi_colon;
	line = str::lremove(line, 1);
	res.push_back(token);
      } else if (line[0] == '-'){
	token.value = str::lpop(line, 1);
	if (line.size() > 1 && line[1] == '>'){
	  token.value += '>';
	  token.type = Token::Type::Returner;
	  line = str::lremove(line, 2);
	} else {
	  token.type = Token::Type::Minus;
	  line = str::lremove(line, 1);
	}
	res.push_back(token);
      } else if (line[0] == '+'){
	token.value = str::lpop(line, 1);
	token.type = Token::Type::Plus;
	line = str::lremove(line, 1);
	res.push_back(token);
      } else if (line[0] == '*'){
	token.value = str::lpop(line, 1);
	token.type = Token::Type::Mult;
	line = str::lremove(line, 1);
	res.push_back(token);
      } else if (line[0] == '{'){
	token.value = str::lpop(line, 1);
	token.type = Token::Type::Open_curl;
	line = str::lremove(line, 1);
	res.push_back(token);
      } else if (line[0] == '}'){
	token.value = str::lpop(line, 1);
	token.type = Token::Type::Close_curl;
	line = str::lremove(line, 1);
	res.push_back(token);
      } else if (line[0] == '='){
	token.value = str::lpop(line, 1);
	token.type = Token::Type::Equal;
	line = str::lremove(line, 1);
	res.push_back(token);
      } else if (line[0] == ' '){
	line = str::lremove(line, 1);
	col++;
      } else if (line[0] == '\n'){
	line = str::lremove(line, 1);
	col = 1;
      } else if (line[0] == '"'){
	// parse opening "
	token.value = str::lpop(line, 1);
	token.type = Token::Type::D_quote;
	token.loc.col = col;
	line = str::lremove(line, 1);
	res.push_back(token);
	col++;

	// parse string
	token.value = str::lpop_until(line, [](const char& ch) { return ch == '"'; });
	token.loc.col = col;
	token.type = Token::Type::String;
	line = str::lremove(line, token.value.size());
	res.push_back(token);
	col += int(token.value.size());

	// parse closing "
	token.value = str::lpop(line, 1);
	token.loc.col = col;
	token.type = Token::Type::D_quote;
	line = str::lremove(line, 1);
	res.push_back(token);
	col++;
      } else if (line[0] == '\''){
	// parse opening quote
	token.value = str::lpop(line, 1);
	token.loc.col = col;
	token.type = Token::Type::Quote;
	line = str::lremove(line, token.value.size());
	res.push_back(token);
	col++;

	// parse char
	token.value = str::lpop(line, 1);
	token.loc.col = col;
	token.type = Token::Type::Char;
	line = str::lremove(line, token.value.size());
	res.push_back(token);
	col++;

	// parse closing quote
	token.value = str::lpop(line, 1);
	token.loc.col = col;
	token.type = Token::Type::Quote;
	line = str::lremove(line, token.value.size());
	res.push_back(token);
	col++;
      } else {
	fprint(std::cerr, "ERROR: Cannot parse `{}`\n", line[0]);
	exit(1);
      }
      if (token.type != Token::Type::D_quote &&
	  token.type != Token::Type::Quote)
	col += int(token.value.size());
    }
    row++;  
  }

  return res;
}

void parse_tokens(std::vector<Token>& tokens){
  for (auto& t : tokens){
    
  }
}

void dump_tokens(std::vector<Token>& tokens){
  print("Tokens:\n");
  for (auto& t : tokens){
    print("{}:{}\n", t.loc.as_str(), t.as_str());
  }
}

int main(int argc, char *argv[]) {
  // for (size_t i = 0; i <= 127; ++i){
  //   print("`{}` {}\n", char(i), (ch::isdigit(char(i)) ? "is digit" : ""));
  // }

  // return 0;

  std::vector<Token> tokens = parse_source_file("main.hash");
  dump_tokens(tokens);

  return 0;
}
