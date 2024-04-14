package analysis

func createGenericListStr(structName string, generics []string) string {
	str := structName + "<"

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
	return t.str() == expected.str()

	// Later support complex and unioin type better.
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
