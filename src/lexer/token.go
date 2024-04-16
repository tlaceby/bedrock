package lexer

import "fmt"

type TokenKind int

const (
	EOF TokenKind = iota
	NUMBER
	STRING
	IDENTIFIER

	// Grouping & Braces
	OPEN_BRACKET
	CLOSE_BRACKET
	OPEN_CURLY
	CLOSE_CURLY
	OPEN_PAREN
	CLOSE_PAREN

	// Equivilance
	ASSIGNMENT
	EQUALS
	NOT_EQUALS
	NOT

	// Conditional
	LESS
	LESS_EQUALS
	GREATER
	GREATER_EQUALS

	// Logical
	OR
	AND

	// Symbols
	DOT
	DOT_DOT
	SEMI_COLON
	COLON
	COLON_COLON
	QUESTION
	COMMA
	AMPERSAND
	ARROW

	OPEN_GENERIC
	CLOSE_GENERIC

	// Shorthand
	PLUS_PLUS
	MINUS_MINUS
	PLUS_EQUALS
	MINUS_EQUALS
	SLASH_EQUALS
	STAR_EQUALS
	NULLISH_ASSIGNMENT // ??=

	//Maths
	PLUS
	DASH
	SLASH
	STAR
	PERCENT

	// Reserved Keywords
	LET
	CONST
	STRUCT
	IMPORT
	TRAIT
	UNSAFE
	TYPE
	AS
	FROM
	FN
	INTERFACE
	IF
	ELSE
	WHILE
	FOR
	TYPEOF
	IN
	RETURN
	BREAK
	CONTINUE
	STATIC
	MATCH
	TYPEDEF

	// macros
	ASSERT_MACRO
	STRING_MACRO

	// Misc
	NUM_TOKENS
)

var reserved_lu map[string]TokenKind = map[string]TokenKind{
	"let":       LET,
	"const":     CONST,
	"struct":    STRUCT,
	"import":    IMPORT,
	"from":      FROM,
	"interface": INTERFACE,
	"fn":        FN,
	"if":        IF,
	"else":      ELSE,
	"return":    RETURN,
	"continue":  CONTINUE,
	"break":     BREAK,
	"static":    STATIC,
	"while":     WHILE, // not used
	"for":       FOR,
	"as":        AS,
	"trait":     TRAIT,
	"typeof":    TYPEOF,
	"in":        IN,
	"type":      TYPE,
	"unsafe":    UNSAFE,
	"match":     MATCH,
	"typedef":   TYPEDEF,

	// Operators as keywords
	"lt":  LESS,
	"lte": LESS_EQUALS,
	"gt":  GREATER,
	"gte": GREATER_EQUALS,
	"ne":  NOT_EQUALS,
	"not": NOT,
	"eq":  EQUALS,
	"and": AND,
	"or":  OR,

	// Reserved Macros
	"@assert": ASSERT_MACRO,
	"@string": STRING_MACRO,
}

type Location struct {
	Line  int
	Start int
	End   int
}

func (l Location) String() string {
	return fmt.Sprintf("Line=%d | Start= %d | End=%d", l.Line, l.Start, l.End)
}

type Token struct {
	Kind     TokenKind
	Value    string
	Location Location
}

func (tk Token) IsOneOfMany(expectedTokens ...TokenKind) bool {
	for _, expected := range expectedTokens {
		if expected == tk.Kind {
			return true
		}
	}

	return false
}

func (token Token) Debug() {
	print(token.String())
}

func (token Token) String() string {
	locStr := token.Location.String()
	if token.Kind == IDENTIFIER || token.Kind == NUMBER || token.Kind == STRING {
		return fmt.Sprintf("%s [%s](%s)\n", locStr, TokenKindString(token.Kind), token.Value)
	} else {
		return fmt.Sprintf("%s [%s]()\n", locStr, TokenKindString(token.Kind))
	}
}

func TokenKindString(kind TokenKind) string {
	switch kind {
	case EOF:
		return "eof"
	case NUMBER:
		return "number"
	case STRING:
		return "string"
	case IDENTIFIER:
		return "identifier"
	case OPEN_BRACKET:
		return "open_bracket"
	case CLOSE_BRACKET:
		return "close_bracket"
	case OPEN_CURLY:
		return "open_curly"
	case CLOSE_CURLY:
		return "close_curly"
	case OPEN_PAREN:
		return "open_paren"
	case CLOSE_PAREN:
		return "close_paren"
	case ASSIGNMENT:
		return "assignment"
	case EQUALS:
		return "equals"
	case NOT_EQUALS:
		return "not_equals"
	case NOT:
		return "not"
	case LESS:
		return "less"
	case LESS_EQUALS:
		return "less_equals"
	case GREATER:
		return "greater"
	case GREATER_EQUALS:
		return "greater_equals"
	case STAR_EQUALS:
		return "star_equals"
	case SLASH_EQUALS:
		return "slash_equals"
	case OR:
		return "or"
	case AND:
		return "and"
	case DOT:
		return "dot"
	case DOT_DOT:
		return "dot_dot"
	case ARROW:
		return "arrow"
	case SEMI_COLON:
		return "semi_colon"
	case COLON:
		return "colon"
	case COLON_COLON:
		return "colon_colon"
	case QUESTION:
		return "question"
	case AMPERSAND:
		return "ampersand"
	case COMMA:
		return "comma"
	case PLUS_PLUS:
		return "plus_plus"
	case MINUS_MINUS:
		return "minus_minus"
	case PLUS_EQUALS:
		return "plus_equals"
	case MINUS_EQUALS:
		return "minus_equals"
	case NULLISH_ASSIGNMENT:
		return "nullish_assignment"
	case PLUS:
		return "plus"
	case DASH:
		return "dash"
	case SLASH:
		return "slash"
	case STAR:
		return "star"
	case PERCENT:
		return "percent"
	case LET:
		return "let"
	case CONST:
		return "const"
	case STRUCT:
		return "struct"
	case IMPORT:
		return "import"
	case AS:
		return "as"
	case TRAIT:
		return "trait"
	case FROM:
		return "from"
	case FN:
		return "fn"
	case IF:
		return "if"
	case ELSE:
		return "else"
	case FOR:
		return "for"
	case WHILE:
		return "while"
	case IN:
		return "in"
	case INTERFACE:
		return "interface"
	case TYPE:
		return "type"
	case UNSAFE:
		return "unsafe"
	case RETURN:
		return "return"
	case CONTINUE:
		return "continue"
	case STATIC:
		return "static"
	case BREAK:
		return "break"
	case MATCH:
		return "match"
	case TYPEDEF:
		return "typedef"

	case OPEN_GENERIC:
		return "open_generic"
	case CLOSE_GENERIC:
		return "close_generic"

		// Macros
	case ASSERT_MACRO:
		return "@assert"
	case STRING_MACRO:
		return "@string"
	default:
		return fmt.Sprintf("unknown(%d)", kind)
	}
}

func newUniqueToken(kind TokenKind, value string, Location Location) Token {
	return Token{
		kind, value, Location,
	}
}

// github.com/sanity-io/litter
