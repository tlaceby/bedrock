#include "lookup.h"
#include "parser.h"

using namespace parser;
using namespace ast;
using namespace lexer;

shared_ptr<ast::LogMacro> parser::parse_log_macro(Parser &p) {
  p.expect(lexer::LOG_MACRO);
  p.expect(lexer::OPEN_PAREN);
  auto expr = make_shared<LogMacro>();
  expr->expr = parse_expr(p, DEFAULT_BP);

  p.expect(lexer::CLOSE_PAREN);
  return expr;
}

shared_ptr<ast::NumMacro> parser::parse_num_macro(Parser &p) {
  p.expect(lexer::NUMBER_MACRO);
  p.expect(lexer::OPEN_PAREN);
  auto expr = make_shared<NumMacro>();
  expr->expr = parse_expr(p, DEFAULT_BP);

  p.expect(lexer::CLOSE_PAREN);
  return expr;
}

shared_ptr<ast::StrMacro> parser::parse_str_macro(Parser &p) {
  p.expect(lexer::STRING_MACRO);
  p.expect(lexer::OPEN_PAREN);
  auto expr = make_shared<StrMacro>();
  expr->expr = parse_expr(p, DEFAULT_BP);

  p.expect(lexer::CLOSE_PAREN);
  return expr;
}

shared_ptr<ast::FmtMacro> parser::parse_fmt_macro(Parser &p) {
  p.expect(lexer::FMT_MACRO);
  p.expect(lexer::OPEN_PAREN);
  auto expr = make_shared<FmtMacro>();
  expr->formatString = p.expect(lexer::STRING).value;

  if (p.current_tk_kind() != COMMA) {
    auto err = Err(ErrKind::InvalidMacroUsage)
                   .location(p.current_tk().pos)
                   .message("Expected to find atleast one argument inside @fmt() call")
                   .hint(cyan("@fmt") +
                         "() expected a formatString and atleast one parameter as a argument but recieved none");

    p.report(err);
  }

  p.expect(COMMA);
  while (p.has_tokens() && p.current_tk_kind() != CLOSE_PAREN) {
    expr->args.push_back(parse_expr(p, DEFAULT_BP));

    if (p.current_tk_kind() != lexer::CLOSE_PAREN) {
      p.expect(lexer::COMMA);
    }
  }

  p.expect(lexer::CLOSE_PAREN);
  return expr;
}
