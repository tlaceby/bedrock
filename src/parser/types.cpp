#include "lookup.h"
#include "parser.h"

using namespace parser;
using namespace ast;
using namespace lexer;

type_nud_handler parser::get_type_nud(Parser& p) {
  auto tk = p.current_tk();
  auto it = type_nud_lu.find(tk.kind);

  if (it != type_nud_lu.end()) {
    return type_nud_lu.at(tk.kind);
  }

  auto err = bad_lu_handler_err("type_nud()", tk);
  p.expect();  // advance past char since it has no null
  p.report(err);
  return nullptr;
}

type_led_handler parser::get_type_led(Parser& p) {
  auto tk = p.current_tk();
  auto it = type_led_lu.find(tk.kind);

  if (it != type_led_lu.end()) {
    return type_led_lu.at(tk.kind);
  }

  auto err = bad_lu_handler_err("type_led()", tk);
  p.expect();  // advance past char since it has no null
  p.report(err);
  return nullptr;
}

shared_ptr<Type> parser::parse_type(Parser& p, BindingPower bp) {
  shared_ptr<Type> left;
  type_nud_handler nud = get_type_nud(p);

  left = nud(p);

  while (type_bp_lu[p.current_tk_kind()] > bp) {
    type_led_handler led = get_type_led(p);
    left = led(p, left, type_bp_lu[p.current_tk_kind()]);
  }

  return left;
}

shared_ptr<SymbolType> parser::parse_symbol_type(Parser& p) {
  auto symbol = make_shared<SymbolType>();
  symbol->symbol = p.expect(lexer::IDENTIFIER).value;
  return symbol;
}

shared_ptr<SliceType> parser::parse_slice_type(Parser& p) {
  auto slice_type = make_shared<SliceType>();
  p.expect(lexer::OPEN_BRACKET);
  p.expect(lexer::CLOSE_BRACKET);
  slice_type->type = parse_type(p, DEFAULT_BP);
  return slice_type;
}

shared_ptr<FnType> parser::parse_fn_type(Parser& p) {
  auto fn = make_shared<FnType>();
  p.expect(lexer::FN);
  auto [params, variadic] = parse_fn_params(p);

  fn->params = params;
  fn->variadic = variadic;

  if (p.current_tk_kind() == ARROW) {
    p.expect(ARROW);
    fn->return_type = parse_type(p, DEFAULT_BP);
  }

  return fn;
}

pair<vector<PropertyKey>, bool> parser::parse_fn_params(Parser& p) {
  vector<PropertyKey> params;
  bool variadic = false;

  p.expect(OPEN_PAREN);
  while (p.has_tokens() && p.current_tk_kind() != CLOSE_PAREN) {
    PropertyKey param;

    // Handle non variadic parameter
    if (p.current_tk_kind() != lexer::DYN) {
      param.name = p.expect(IDENTIFIER).value;
      p.expect(COLON);
      param.type = parse_type(p, DEFAULT_BP);

      if (p.current_tk_kind() != CLOSE_PAREN) {
        p.expect(COMMA);
      }

      params.push_back(param);
      continue;
    }

    p.expect(DYN);
    param.name = p.expect(IDENTIFIER).value;
    param.variadic = true;
    p.expect(COLON);
    param.type = parse_type(p, DEFAULT_BP);

    if (param.type->kind != SLICE_TYPE) {
      Err err{ErrKind::InvalidVariadicDeclaration};
      err.message("Expected []T inside variadic parameter definition");
      err.hint("Example: dyn args: []T");
      err.location(p.current_tk().pos);
      p.report(err);
    }

    if (p.current_tk_kind() != CLOSE_PAREN) {
      Err err{ErrKind::InvalidVariadicDeclaration};
      err.message("expected closing parenthesis following declaration.");
      err.hint("Example: fn (a: T, b: T, dyn args: []T)");
      err.hint("Missing closing parenthesis following slice type.");
      err.location(p.current_tk().pos);
      p.report(err);
    }

    params.push_back(param);
    variadic = true;
  }

  p.expect(CLOSE_PAREN);
  return make_pair(params, variadic);
}