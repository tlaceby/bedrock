#include "../ast/ast_stmt.h"
#include "analsyis.h"
#include "types.h"

using namespace analysis;
using namespace ast;

shared_ptr<Scope> createGlobalScope() {
  auto env = make_shared<Scope>();
  env->is_global = true;
  env->name = "global";

  // Define Types
  env->defineType("Bool", MK_BOOL());
  env->defineType("Number", MK_NUM());
  env->defineType("Void", MK_VOID());
  env->defineType("String", MK_STR());

  // Define program object
  auto p = MK_STRUCT("BedrockProgram");
  p->properties["argc"] = MK_NUM();
  p->properties["cwd"] = MK_STR();

  env->defineType("BedrockProgram", p);
  env->defineSymbol("program", p, true);

  // Define Global Variables
  env->defineSymbol("true", MK_BOOL(), true);
  env->defineSymbol("false", MK_BOOL(), true);

  // Define Modules

  // Define Macros

  // etc...

  return env;
}

shared_ptr<analysis::Type> analysis::tc_program(
    shared_ptr<ast::ProgramStmt> stmt) {
  Scope::global = createGlobalScope();

  for (const auto& module : stmt->modules) {
    tc_module(module);
  }

  Scope::debugAllScopes();
  return MK_VOID();
}

shared_ptr<analysis::Type> analysis::tc_module(
    shared_ptr<ast::ModuleStmt> stmt) {
  auto env = make_shared<Scope>();

  env->is_module = true;
  env->name = stmt->name;

  stmt->scope = env;

  for (const auto& s : stmt->body) {
    tc_stmt(s, stmt->scope);
  }

  stmt->scope->debugScope();
  return MK_VOID();
}

shared_ptr<analysis::Type> analysis::tc_var_decl_stmt(VarDeclStmt* stmt,
                                                      shared_ptr<Scope> env) {
  auto varname = stmt->varname;
  auto vartype = stmt->type ? tc_type(stmt->type, env) : nullptr;

  if (env->symbolExists(varname)) {
    printf("Variable already exists in the current scope %s\n",
           varname.c_str());
    exit(1);
  }

  // No initial value
  if (vartype && !stmt->value) {
    env->defineSymbol(varname, vartype);
    return vartype;
  }

  auto recievedType = tc_expr(stmt->value, env);

  // Infer type usage
  if (!vartype && recievedType) {
    env->defineSymbol(varname, recievedType);
    return recievedType;
  }

  // Check expected is recieved
  if (types_match(vartype, recievedType)) {
    env->defineSymbol(varname, vartype);
    return vartype;
  }

  printf("Variable declation for %s expected %s but recieved %s instead.\n",
         varname.c_str(), vartype->str().c_str(), recievedType->str().c_str());
  exit(1);
}

shared_ptr<analysis::Type> analysis::tc_fn_decl_stmt(FnDeclStmt* stmt,
                                                     shared_ptr<Scope> env) {
  auto fnname = stmt->name;
  auto returns =
      stmt->return_type ? tc_type(stmt->return_type, env) : MK_VOID();
  auto fnEnv = make_shared<Scope>();
  auto params = vector<FnParam>();

  fnEnv->parent = env;
  fnEnv->is_function = true;
  fnEnv->name = env->name + "." + fnname + "()";

  // Make sure function does not already exist
  if (env->resolveSymbol(fnname)) {
    printf(
        "Function %s redeclared. Cannot have multiple declarations of the same "
        "function.",
        fnname.c_str());
    exit(1);
  }

  // Build Parameters
  for (const auto& param : stmt->params) {
    auto paramType = tc_type(param.type, env);
    fnEnv->defineSymbol(param.name, paramType, true);
    params.push_back(FnParam(param.name, paramType));
  }

  auto fn = MK_FN(params, returns, false);
  env->defineSymbol(fnname, fn);

  // Todo install function in env for recursive calls.

  stmt->body->scope = fnEnv;
  tc_block_stmt(stmt->body.get());

  fnEnv->debugScope();
  return MK_VOID();
}

shared_ptr<analysis::Type> analysis::tc_block_stmt(BlockStmt* stmt) {
  auto env = stmt->scope;

  for (const auto& s : stmt->body) {
    tc_stmt(s, env);
  }

  return MK_VOID();
}

shared_ptr<analysis::Type> analysis::tc_expr_stmt(ExprStmt* stmt,
                                                  shared_ptr<Scope> env) {
  return tc_expr(stmt->expr, env);
}

shared_ptr<analysis::Type> analysis::tc_struct_stmt(StructStmt* stmt,
                                                    shared_ptr<Scope> env) {
  auto name = stmt->name;
  auto s = MK_STRUCT(name);

  // Verify name does not already exist
  if (env->typeExists(name)) {
    std::cout << "Cannot define struct " << name;
    std::cout << " as a type with this name already exists\n";
    exit(1);
  }

  // Install & Validate Properties
  for (const auto& prop : stmt->properties) {
    auto propName = prop.name;
    auto propType = tc_type(prop.type, env);

    // Name is already in use
    if (s->hasField(propName)) {
      std::cout << "Struct " << name << " ";
      std::cout << "already has a property with ";
      std::cout << "the name " << propName << "\n";
      exit(1);
    }

    if (prop.is_static) {
      s->staticProperties[propName] = propType;
    } else {
      s->properties[propName] = propType;
    }

    if (prop.is_pub) {
      s->publicMembers.insert(propName);
    }
  }

  // Add struct to current scope if inside module or global
  if (env->is_module || env->is_global) {
    env->defineType(name, s);
    return s;
  }

  std::cout << "Struct " << name << " ";
  std::cout << "cannot be declared in current scope\n";
  exit(1);
}

shared_ptr<analysis::Type> analysis::tc_stmt(shared_ptr<ast::Stmt> stmt,
                                             shared_ptr<Scope> env) {
  switch (stmt->kind) {
    case VAR_DECL_STMT:
      return tc_var_decl_stmt(static_cast<VarDeclStmt*>(stmt.get()), env);
    case FN_DECL_STMT:
      return tc_fn_decl_stmt(static_cast<FnDeclStmt*>(stmt.get()), env);
    case EXPR_STMT:
      return tc_expr_stmt(static_cast<ExprStmt*>(stmt.get()), env);
    case STRUCT_STMT:
      return tc_struct_stmt(static_cast<StructStmt*>(stmt.get()), env);
    default:
      stmt->debug(0);
      std::cout << "^^^^^ typechecking for node Unimplimented ^^^^^^\n";
      TODO("Unimplimented Typechecking for stmt");
  }
}
