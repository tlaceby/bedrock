package lexer

import "fmt"

type TokenKind int

const (
	EOF TokenKind = iota
	NULL
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
	QUESTION
	COMMA
	AMPERSAND

	// Shorthand
	PLUS_PLUS
	MINUS_MINUS
	PLUS_EQUALS
	MINUS_EQUALS
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
	NEW
	IMPORT
	FROM
	FN
	INTERFACE
	IF
	ELSE
	FOREACH
	WHILE
	FOR
	EXPORT
	TYPEOF
	IN

	// Misc
	NUM_TOKENS
)

var reserved_lu map[string]TokenKind = map[string]TokenKind{
	"let":     LET,
	"const":   CONST,
	"struct":   STRUCT,
	"new":     NEW,
	"import":  IMPORT,
	"from":    FROM,
	"interface": INTERFACE,
	"fn":      FN,
	"if":      IF,
	"else":    ELSE,
	"foreach": FOREACH,
	"while":   WHILE, // not used
	"for":     FOR, // not used
	"export":  EXPORT, // not used
	"typeof":  TYPEOF,
	"in":  IN,
}

type Location struct {
	Line int
	Start int
	End int
}

func (l Location) String () string {
	return fmt.Sprintf("Line=%d | Start= %d | End=%d", l.Line, l.Start, l.End)
}

type Token struct {
	Kind  TokenKind
	Value string
	Location Location
}

func (tk Token) IsOneOfMany (expectedTokens ...TokenKind) bool {
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

func (token Token) String () string {
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
	case NULL:
		return "null"
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
	case OR:
		return "or"
	case AND:
		return "and"
	case DOT:
		return "dot"
	case DOT_DOT:
		return "dot_dot"
	case SEMI_COLON:
		return "semi_colon"
	case COLON:
		return "colon"
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
	case NEW:
		return "new"
	case IMPORT:
		return "import"
	case FROM:
		return "from"
	case FN:
		return "fn"
	case IF:
		return "if"
	case ELSE:
		return "else"
	case FOREACH:
		return "foreach"
	case FOR:
		return "for"
	case WHILE:
		return "while"
	case IN:
		return "in"
	case INTERFACE:
		return "interface"
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