#include "analsyis.h"

using namespace analysis;
using namespace ast;

shared_ptr<analysis::Type> analysis::tc_type(shared_ptr<ast::Type> type,
                                             shared_ptr<analysis::Scope> env) {
  switch (type->kind) {
    case SYMBOL_TYPE: {
      auto typeName = static_cast<SymbolType*>(type.get())->symbol;
      auto foundType = env->resolveType(typeName);

      if (!foundType) {
        printf("Type %s does not exist.\n", typeName.c_str());
        exit(1);
      }

      return foundType;
    }
    case POINTER_TYPE:
      return MK_PTR(
          tc_type(static_cast<ast::PointerType*>(type.get())->type, env));

    default:
      type->debug(0);
      std::cout << "^^^^^ typechecking for node Unimplimented ^^^^^^\n";
      TODO("Unimplimented Typechecking for type");
      break;
  }
}
