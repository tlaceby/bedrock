package analysis

import (
	"fmt"

	"github.com/tlaceby/bedrock/src/ast"
	"github.com/tlaceby/bedrock/src/helpers"
)

func tc_symbol_type(t ast.SymbolType, env *SymbolTable) Type {
	_type, _ := env.findNearestTypeEnv(t.Value)
	return _type
}

func tc_list_type(t ast.ListType, env *SymbolTable) Type {
	return ArrayType{
		Underlying: typecheck_type(t.Underlying, env),
	}
}

func tc_fn_type(t ast.FnType, env *SymbolTable) Type {
	var params = []Type{}
	var returns = typecheck_type(t.ReturnType, env)

	for _, param := range t.Parameters {
		params = append(params, typecheck_type(param, env))
	}

	return FnType{
		Variadic:   false,
		ParamTypes: params,
		ReturnType: returns,
	}
}

func tc_struct_generic_type(t ast.StructType, env *SymbolTable) Type {
	var genericEnv *SymbolTable
	var structSignature string // StructName<T, ..>{}
	var structName = t.StructName
	var genStruct GenericStructType
	var expectedGenericArity int
	var expectedGenericNames []string
	var recievedGenericArity = len(t.GenericList)
	var recievedGenerics = []Type{}
	var recievedGenericsTypeStrings = []string{}
	var T, definedEnv = env.findNearestTypeEnv(structName)

	if !IsAStructType(T) {
		panic(fmt.Sprintf("Could not resolve struct with name %s.", structName))
	}

	if IsNormalStructType(T) {
		panic(fmt.Sprintf("Struct %s is not a generic struct. No need for type parameters.", structName))
	} else {
		genStruct = helpers.ExpectType[GenericStructType](T)
		expectedGenericArity = len(genStruct.Generics)
		expectedGenericNames = genStruct.Generics
	}

	// Verify Arity Matches Expected
	if expectedGenericArity != recievedGenericArity {
		panic(fmt.Sprintf("Generic %s expected arity of %d but recieved %d instead.", structName, expectedGenericArity, recievedGenericArity))
	}

	// Calculate Generics Recieved
	for _, recievedGeneric := range t.GenericList {
		genericType := typecheck_type(recievedGeneric, env)
		recievedGenerics = append(recievedGenerics, genericType)
		recievedGenericsTypeStrings = append(recievedGenericsTypeStrings, genericType.str())
	}

	structSignature = createGenericListStr(structName, recievedGenericsTypeStrings)
	genericEnv = CreateSymbolTable(env, false, false, true, structSignature)

	// Install Generic Types
	for indx, genericName := range expectedGenericNames {
		recieved := recievedGenerics[indx]
		genericEnv.DefinedTypes[genericName] = recieved
	}

	// Generate Valid Struct Instance Which is NOT Generic
	// This can Be installed in the environment which it was defined.
	structInstanceType := validate_struct_body(genericEnv, structSignature, genStruct.Properties, genStruct.StaticMethods, genStruct.InstanceMethods)
	definedEnv.DefinedTypes[structSignature] = structInstanceType
	return structInstanceType
}
