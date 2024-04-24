package analysis

func typeError(kind string, message string) TypeError {
	return TypeError{
		Kind:    kind,
		Message: message,
	}
}
