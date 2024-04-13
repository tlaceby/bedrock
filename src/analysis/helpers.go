package analysis

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

func IsNormalFnType(t Type) bool {
	switch t.(type) {
	case FnType:
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
