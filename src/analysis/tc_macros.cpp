#include "analsyis.h"

#include <regex>
using std::regex;
using std::smatch;

using namespace analysis;
using namespace ast;

shared_ptr<analysis::Type> analysis::tc_log_macro(ast::LogMacro *macro, shared_ptr<analysis::Scope> env) {
  auto arg = tc_expr(macro->expr, env);

  if (types_match(MK_NUM(), arg) || types_match(MK_BOOL(), arg) || types_match(MK_STR(), arg)) {
    return MK_VOID();
  }

  std::cout << "@log(" << arg->str() << ") is invalid. Expected @log(" << MK_STR()->str() << ")\n";
  exit(1);
}

shared_ptr<analysis::Type> analysis::tc_fmt_macro(ast::FmtMacro *macro, shared_ptr<analysis::Scope> env) {
  const auto pattern = regex(R"(\{\})");
  const auto fmtStr = macro->formatString;
  const auto numberMatches = utils::countMatchInRegex(fmtStr, pattern);
  const auto numberArgs = macro->args.size();

  if (numberMatches == 0) {
    std::cout << cyan("@fmt") << "() expects atleast one format parameter inside formatString. Recieved 0 instead\n";
    exit(1);
  }

  if (numberMatches != numberArgs) {
    std::cout << cyan("@fmt") << "(" << bold_yellow("\"") << yellow(fmtStr) << bold_yellow("\"") << ")";
    std::cout << " expected " << numberMatches << " args to match template but recieved " << numberArgs << " instead\n";
    exit(1);
  }

  for (const auto &arg : macro->args) {
    const auto argType = tc_expr(arg, env);

    if (!types_match(MK_STR(), argType)) {
      std::cout << "Argument " << argType->str() << " inside @fmt() is not a String\n";
      exit(1);
    }
  }

  return MK_STR();
}

shared_ptr<analysis::Type> analysis::tc_str_macro(ast::StrMacro *macro, shared_ptr<analysis::Scope> env) {
  auto arg = tc_expr(macro->expr, env);

  if (types_match(MK_NUM(), arg) || types_match(MK_BOOL(), arg)) {
    return MK_STR();
  }

  std::cout << "@str(" << arg->str() << ") is invalid. ";
  std::cout << "Expected @str(" << MK_NUM()->str() << "|" << MK_BOOL()->str() << ")\n";
  exit(1);
}

shared_ptr<analysis::Type> analysis::tc_num_macro(ast::NumMacro *macro, shared_ptr<analysis::Scope> env) {
  auto arg = tc_expr(macro->expr, env);

  if (types_match(MK_STR(), arg) || types_match(MK_BOOL(), arg)) {
    return MK_NUM();
  }

  std::cout << "@num(" << arg->str() << ") is invalid. Expected @num(" << MK_STR()->str() << ")\n";
  exit(1);
}
