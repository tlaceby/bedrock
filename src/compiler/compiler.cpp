#include "compiler.h"

using namespace compiler;
using namespace ast;

void Compiler::compile(shared_ptr<ProgramStmt> program, string outpath) {
  depth = 0;

  for (const auto &mod : program->modules) {
    if (!mod->is_entry) {
      compile_module(mod.get());
    }
  }

  compile_module(program->entry.get());
  debugBytecode();
}

void Compiler::compile_stmt(shared_ptr<Stmt> stmt, shared_ptr<analysis::Scope> env) {

  switch (stmt->kind) {
  case MODULE_STMT:
    return compile_module(static_cast<ModuleStmt *>(stmt.get()));
  case BLOCK_STMT:
    return compile_block_stmt(static_cast<BlockStmt *>(stmt.get()), env);
  case VAR_DECL_STMT:
    return compile_var_decl_stmt(static_cast<VarDeclStmt *>(stmt.get()), env);
  case FN_DECL_STMT:
    return compile_fn_decl_stmt(static_cast<FnDeclStmt *>(stmt.get()), env);
  case EXPR_STMT:
    return compile_expr_stmt(static_cast<ExprStmt *>(stmt.get()), env);
  case RETURN_STMT:
    return compile_return_stmt(static_cast<ReturnStmt *>(stmt.get()), env);
  }

  std::cout << "Compile::stmt() unknown kind: " << stmt->kind << "\n";
  exit(1);
}

void Compiler::compile_expr(shared_ptr<Expr> expr, shared_ptr<analysis::Scope> env) {
  switch (expr->kind) {
  case NUMBER_EXPR:
    return compile_number_expr(static_cast<NumberExpr *>(expr.get()), env);
  case STRING_EXPR:
    return compile_string_expr(static_cast<StringExpr *>(expr.get()), env);
  case SYMBOL_EXPR:
    return compile_symbol_expr(static_cast<SymbolExpr *>(expr.get()), env);
  case BINARY_EXPR:
    return compile_binary_expr(static_cast<BinaryExpr *>(expr.get()), env);

  // Macros
  case LOG_MACRO:
    return compile_log_macro(static_cast<LogMacro *>(expr.get()), env);
  }

  std::cout << "Compile::expr() unknown kind: " << expr->kind << "\n";
  exit(1);
}

size_t Compiler::getConstantAddr(double d) {
  for (size_t addr = 0; addr < data.size(); addr++) {
    auto val = data[addr];
    if (val.kind == runtime::Number && val.data.number == d) {
      return addr;
    }
  }

  data.push_back(runtime::Val(d));
  return data.size() - 1;
}

size_t Compiler::getConstantAddr(string s) {
  TODO("Unimplimented getConstantAddr(string)");
}

size_t Compiler::getConstantAddr(bool b) {
  for (size_t addr = 0; addr < data.size(); addr++) {
    auto val = data[addr];
    if (val.kind == runtime::Boolean && val.data.boolean == b) {
      return addr;
    }
  }

  data.push_back(runtime::Val(b));
  return data.size() - 1;
}

void Compiler::debugBytecode() {
  string instructions = "";

  size_t w = 0;
  for (const auto &instr : code) {
    instructions += to_string(instr) + " ";

    if (w == 5) {
      instructions += "\n";
      w = 0;
    }

    w++;
  }

  using std::cout, std::hex;
  using namespace runtime;
  cout << "\n-----------------------------------------\n" << instructions << "\n\n";
  using std::cout;
  using namespace runtime;

  // Data section
  cout << "\n.data:\n";

  for (size_t addr = 0; addr < data.size(); addr++) {
    const auto val = data[addr];
    printf("%03lu  ", addr);

    switch (val.kind) {
    case Number:
      cout << "number ";
      cout << val.data.number << "\n";
      break;

    default:
      TODO("Debug Bytecode print_value() not implimented");
    }
  }

  // Constants Section
  cout << "\n.globals:\n";

  for (const auto &[varname, addr] : globals_lu) {
    printf("%03lu  ", addr);
    std::cout << varname << "\n";
  }

  // Instructions
  size_t ip = 0;
  cout << "\n.instructions:\n";

  while (ip < code.size()) {
    printf("%04lu  ", ip);

    switch (code[ip++]) {
    case op_hlt:
      cout << "(hlt) halt";
      break;
    case op_loadg:
      cout << "(loadg) load_global " << code[ip] << "  #" << getGlobalFromAddr(code[ip++]);
      break;
    case op_loadl:
      cout << "(loadl) load_local " << code[ip++];
      break;
    case op_push:
      cout << "(push) push " << code[ip++] << " ";
      // logRuntimeValue(data[code[ip++]]);
      break;
    case op_storeg:
      cout << "(storeg) store_global " << code[ip] << "  " << getGlobalFromAddr(code[ip++]);
      break;
    case op_storel:
      cout << "(storel) store_local " << code[ip++];
      break;
    case op_incsp:
      cout << "(incsp) incriment_stack_pointer " << code[ip] << "\n";
      break;
    case op_decsp:
      cout << "(decsp) decrement_stack_pointer " << code[ip++] << "\n";
      break;
    case op_jmp:
      cout << "(jmp) jump " << code[ip++];
      break;
    case op_call:
      cout << "(call) call " << code[ip++];
      break;
    case op_ret:
      cout << "(ret) return " << code[ip++];
      break;
    case op_add:
      cout << "(add) add";
      break;
    case op_sub:
      cout << "(sub) subtract ";
      break;
    case op_mul:
      cout << "(mul) multiply ";
      break;
    case op_div:
      cout << "(div) divide ";
      break;
    case op_mod:
      cout << "(mod) modulus ";
      break;
    case op_concat:
      cout << "(concat) concat ";
      break;
    default:
      cout << "(unknown) unknown operation " << code[ip - 1];
      break;
    }

    std::cout << "\n";
  }

  cout << "\n-----------------------------------------\n";
}

void Compiler::scope_enter(shared_ptr<analysis::Scope> env) {
  depth++;
  auto numLocals = env->local_offsets.size();

  // Setup Space for Locals
  if (numLocals > 0) {
    emit(op_incsp);
    emit(numLocals);
  }
}

void Compiler::scope_exit(shared_ptr<analysis::Scope> env) {
  auto numLocals = env->local_offsets.size();

  // Setup Space for Locals
  if (numLocals > 0) {
    emit(op_decsp);
    emit(numLocals);
  }

  depth--;
}
