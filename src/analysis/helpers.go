package analysis

import "github.com/fatih/color"

func createGenericListStr(structName string, generics []string) string {
	str := color.MagentaString(structName) + "<"

	for indx, name := range generics {
		str += name
		if indx != len(generics)-1 {
			str += ", "
		}
	}

	str += ">"
	return str
}

func typesSame(t Type, expected Type) bool {
	if t.str() == expected.str() {
		return true
	}

	switch other := expected.(type) {
	case TraitType:
		return implements(t, other)
	}

	return false
}

func CastableToBool(t Type) bool {
	switch t.(type) {
	case BoolType:
		return true
	case NumType:
		return true
	case StrType:
		return true
	case ArrayType:
		return true
	default:
		return false
	}
}

func CastableToString(t Type) bool {
	// For now everything can be cast to a string functions, structs and traits included.
	switch t.(type) {
	case BoolType:
		return true
	case NumType:
		return true
	case StrType:
		return true
	case ArrayType:
		return true
	default:
		return true
	}
}

func IsAFnType(t Type) bool {
	switch t.(type) {
	case FnType:
		return true
	case GenericFnType:
		return true
	}

	return false
}

func IsAStructType(t Type) bool {
	switch t.(type) {
	case StructType:
		return true
	case GenericStructType:
		return true
	}

	return false
}

func IsNormalFnType(t Type) bool {
	switch t.(type) {
	case FnType:
		return true
	}

	return false
}

func IsNormalStructType(t Type) bool {
	switch t.(type) {
	case StructType:
		return true
	}

	return false
}

func IsGenericFnType(t Type) bool {
	switch t.(type) {
	case GenericFnType:
		return true
	}

	return false
}

func IsGenericStructType(t Type) bool {
	switch t.(type) {
	case GenericStructType:
		return true
	}

	return false
}

func implements(t Type, trait TraitType) bool {
	switch v := t.(type) {
	case StructType:
		for methodName, fnType := range trait.Methods {
			method, exists := v.Methods[methodName]
			if !exists || !isFnTypesEqual(method, fnType) {
				return false
			}
		}
		return true
	// Add cases for other types that can implement traits
	default:
		return false
	}
}

func isFnTypesEqual(fn1, fn2 FnType) bool {
	if fn1.Variadic != fn2.Variadic || len(fn1.ParamTypes) != len(fn2.ParamTypes) {
		return false
	}

	for i := range fn1.ParamTypes {
		if !isTypesEqual(fn1.ParamTypes[i], fn2.ParamTypes[i]) {
			return false
		}
	}

	return isTypesEqual(fn1.ReturnType, fn2.ReturnType)
}

func isTypesEqual(t1, t2 Type) bool {
	// Implement type equality check based on your type system
	// You can compare the string representation of types for simplicity
	return t1.str() == t2.str()
}
