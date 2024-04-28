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
