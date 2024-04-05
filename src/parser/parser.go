package parser

import (
	"github.com/tlaceby/bedrock/src/ast"
	"github.com/tlaceby/bedrock/src/lexer"
)

type parser struct {
	filePath string
	source   string
	tokens   []lexer.Token
	pos      int
}

func createParser(tokens []lexer.Token, filePath string) *parser {
	createTokenLookups()
	createTypeTokenLookups()

	p := &parser{
		filePath: filePath,
		source:   "",
		tokens:   tokens,
		pos:      0,
	}

	return p
}

func Parse(source string, filePath string) ast.ModuleStmt {
	tokens := lexer.Tokenize(source, filePath)

	for _, tk := range tokens {
		tk.Debug()
	}

	p := createParser(tokens, filePath)
	p.source = source
	body := make([]ast.Stmt, 0)

	for p.hasTokens() {
		body = append(body, parse_stmt(p))
	}

	return ast.ModuleStmt{
		Body:       body,
		FilePath:   filePath,
		ModuleName: filePath,
	}
}
