#define STDCPP_IMPLEMENTATION
#include <stdcpp.hpp>
#include <stack>
#include <filesystem>
#include <unordered_map>
namespace fs = std::filesystem;

struct Loc{
  int col{0}, row{0};
  std::string file_path;

  std::string as_str() const {
    return FMT("{}:{}:{}", file_path, row, col);
  }
};

struct Token{
  enum class Type{
    Name,
    Number,
    Open_paren,
    Close_paren,
    Semi_colon,
    Colon,
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
    case Type::Colon: {
      return "Colon";
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

typedef std::vector<Token> Tokens;

Option<Token> pop_token(std::vector<Token>& tokens){
  Option<Token> res;
  if (!tokens.empty()){
    res = tokens[0];
    tokens.erase(tokens.begin() + 0);
  }
  return res;  
}

#define compiler_error(tok, str, ...) compiler_error_impl(tok, FMT(str, __VA_ARGS__))

void compiler_error_impl(const Token& token, const std::string& err_msg){
  fprint(std::cerr, "{}: ERROR: {}\n", token.loc.as_str(), err_msg);
  exit(1);
}

#define FILE_EXT "hash"

Tokens parse_source_file(const std::string& filename){
  std::string file_ext = str::rpop_until(filename, '.');
  if (file_ext != FILE_EXT){
    fprint(std::cerr, "ERROR: Hash source files must have the extension `{}`!\n", FILE_EXT);
    exit(1);
  }
  std::string file = file::slurp_file(filename);
  Tokens res;
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
      } else if (line[0] == ':'){
	token.value = str::lpop(line, 1);
	token.type = Token::Type::Colon;
	line = str::lremove(line, token.value.size());
	res.push_back(token);	
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

enum class Keyword {
  Func
};


struct Value {
  enum class Type {
    Int,
    Float,
    Ptr,
    Char,
    Str,
    Bool,
    Count
  } type;

  static inline std::unordered_map<std::string, Type> type_as_name = {
    {"int", Type::Int},
    {"float", Type::Float},
    {"ptr", Type::Ptr},
    {"char", Type::Char},
    {"str", Type::Str},
    {"bool", Type::Bool},    
  };

  static bool is_valid_type(const std::string& n){
    return Value::type_as_name.contains(n);
  }

};

struct Block {
  std::vector<Token> _tokens;

  void collect_values(Token& curl_token, Tokens& tokens){
    Option<Token> T;
    do {
      T = pop_token(tokens);
      if (!T) {
	compiler_error(curl_token, "Unclosed Function body");
      }
      Token token = T.unwrap();
      print("Collected: `{}`\n", token.as_str());
      if (token.type == Token::Type::Close_curl){
	break;
      } else {
	_tokens.push_back(token);
      }
    } while (T);
  }
};


struct Function {
  std::string name;
  std::vector<Value> args;
  Block block;
  Value return_value;
  Token token;
};

std::vector<Function> functions;

static std::unordered_map<std::string, Keyword> keywords = {
  {"func",  Keyword::Func},
};

bool is_keyword(const std::string& name){
  return keywords.contains(name);
}



int parse_arguments(Token& open_paren, Tokens& tokens){
  Option<Token> T;
  std::vector<std::string> declared_arguments;
  int arg_count = 0;
  bool name_handled = false;
  
  do {
    T = pop_token(tokens);
    if (!T){
      compiler_error(open_paren, "Unclosed parenthesis");
    }
    Token t = T.unwrap();
    
    if (t.type == Token::Type::Name){
      bool found = false;
      for (auto& a : declared_arguments){ if (a == t.value) { found = true; break; } }
      if (!found){ declared_arguments.push_back(t.value); }
      name_handled = true;
    } else if (t.type == Token::Type::Close_paren){
      break;
    } else if (t.type == Token::Type::Colon){
      if (!name_handled){
	compiler_error(t, "Argument name is not provided");
      }
      name_handled=false;
      T = pop_token(tokens);
      if (!T){
	// TODO: pass the token of the function
	compiler_error(t, "Unfinished Function declaration");
      }
      t = T.unwrap();
      if (!Value::is_valid_type(t.value)) {
	compiler_error(t, "Unkown type `{}`", t.value);
      }
    } else if (t.type == Token::Type::Comma){
      arg_count++;
    } else {
      compiler_error(t, "`{}` is unexpected here", t.value);
    }
    
  } while (T);
  return arg_count;
}

void parse_tokens(Tokens& tokens){
  Option<Token> T;
  Option<Token> prev;

  Function current_func;
  bool declaring_func=false;
  
  do {
    T = pop_token(tokens);
    Token token = T.unwrap();
    
    switch (token.type){
    case Token::Type::Name: {
      if (is_keyword(token.value)){
	Keyword k = keywords[token.value];
	switch(k){
	case Keyword::Func:{
	  
	} break;
	default:{
	  UNREACHABLE();
	} break;
	}
      }
    } break;
    case Token::Type::Number: {
      UNIMPLEMENTED();
    } break;
    case Token::Type::Open_paren: {
      declaring_func = true;
      // TODO: check if the previous token was a name
      if (!prev) {
	compiler_error(token, "Function has no name");
      }
      
      current_func.name = prev.unwrap().value;
      int arg_count = parse_arguments(token, tokens);
      current_func.args.resize(arg_count);
      current_func.token = prev.unwrap();
      // TODO: actually get the function args somehow
      
    } break;
    case Token::Type::Close_paren: {
      UNIMPLEMENTED();
    } break;
    case Token::Type::Semi_colon: {
      UNIMPLEMENTED();
    } break;
    case Token::Type::Colon: {
      UNIMPLEMENTED();
    } break;
    case Token::Type::Comma: {
      UNIMPLEMENTED();
    } break;
    case Token::Type::Minus: {
      UNIMPLEMENTED();
    } break;
    case Token::Type::Plus: {
      UNIMPLEMENTED();
    } break;
    case Token::Type::Mult: {
      UNIMPLEMENTED();
    } break;
    case Token::Type::Div: {
      UNIMPLEMENTED();
    } break;
    case Token::Type::Mod: {
      UNIMPLEMENTED();
    } break;
    case Token::Type::Equal: {
      UNIMPLEMENTED();
    } break;
    case Token::Type::Returner: {
      T = pop_token(tokens);
      if (!T){
	// TODO: pass the token of the function
	compiler_error(token, "Unfinished Function declaration");
      }
      token = T.unwrap();
      if (!Value::is_valid_type(token.value)) {
	compiler_error(token, "Unkown type `{}`", token.value);
      }
    } break;
    case Token::Type::Open_curl: {
      if (!declaring_func){
	compiler_error(token, "`{}` is unexpected here", token.value);
      }
      
      // collect values of the func block
      current_func.block.collect_values(token, tokens);
    } break;
    case Token::Type::Close_curl: {
      UNIMPLEMENTED();
    } break;
    case Token::Type::D_quote: {
      UNIMPLEMENTED();
    } break;
    case Token::Type::Quote: {
      UNIMPLEMENTED();
    } break;
    case Token::Type::String: {
      UNIMPLEMENTED();
    } break;
    case Token::Type::Char: {
      UNIMPLEMENTED();
    } break;
    default: {
      UNREACHABLE();
    } break;
    }

    prev = T;
  } while (T);
}

void dump_tokens(Tokens& tokens){
  print("Tokens:\n");
  for (auto& token : tokens){
    print("{}:{}\n", token.loc.as_str(), token.as_str());
  }
}

int main(int argc, char *argv[]) {
  // for (size_t i = 0; i <= 127; ++i){
  //   print("`{}` {}\n", char(i), (ch::isdigit(char(i)) ? "is digit" : ""));
  // }

  // return 0;

  Tokens tokens = parse_source_file("main.hash");
  // dump_tokens(tokens);
  parse_tokens(tokens);

  return 0;
}
