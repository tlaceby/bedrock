package parser

import (
	"fmt"
	"os"

	"github.com/tlaceby/bedrock/src/helpers"
	"github.com/tlaceby/bedrock/src/lexer"
)

func (p *parser) currentToken() lexer.Token {
	return p.tokens[p.pos]
}

func (p *parser) prevToken() lexer.Token {
	return p.tokens[p.pos-1]
}

func (p *parser) advance() lexer.Token {
	tk := p.currentToken()
	p.pos++
	return tk
}

func (p *parser) hasTokens() bool {
	return p.pos < len(p.tokens) && p.currentTokenKind() != lexer.EOF
}

func (p *parser) currentTokenKind() lexer.TokenKind {
	return p.tokens[p.pos].Kind
}

func (p *parser) expectError(expectedKind lexer.TokenKind, err string) lexer.Token {
	token := p.currentToken()
	kind := token.Kind

	if kind != expectedKind {
		if len(err) == 0 {
			errMsg := fmt.Sprintf("Expected a %s but recieved %s instead.", lexer.TokenKindString(expectedKind), lexer.TokenKindString(kind))
			loc := p.prevToken().Location
			preview, lineError := helpers.GetCodePreview(p.source, loc.Start, loc.End, loc.Line)
			err = fmt.Sprintf("\nTokenMismatch::Unexpected token found during parsing.\n%s\n\nLoc: %s::%d\nMsg: %s\nErr: %s\n", preview, p.filePath, loc.Line, errMsg, lineError)
		}

		fmt.Print(err)
		os.Exit(1)
	}

	return p.advance()
}

func (p *parser) expect(expectedKind lexer.TokenKind) lexer.Token {
	return p.expectError(expectedKind, "")
}
