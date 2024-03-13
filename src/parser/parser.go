package parser

import (
	"github.com/tlaceby/bedrock/src/ast"
	"github.com/tlaceby/bedrock/src/lexer"
)

type parser struct {
	source string
	tokens []lexer.Token
	pos int
}

func createParser (tokens []lexer.Token) *parser {
	createTokenLookups()
	createTypeTokenLookups()

	p := &parser{
		source: "",
		tokens: tokens,
		pos: 0,
	}

	return p
}

func Parse (source string) ast.BlockStmt {
	tokens := lexer.Tokenize(source)

	for _, tk := range tokens {
		tk.Debug()
	}

	p := createParser(tokens)
	p.source = source
	body := make([]ast.Stmt, 0)

	for p.hasTokens() {
		body = append(body, parse_stmt(p))
	}

	return ast.BlockStmt{
		Body: body,
	}
}
