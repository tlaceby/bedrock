package lexer

import "testing"

const TESTING_SOURCE = "lexer/testing.br"

func TestBlankSource(t *testing.T) {
	source := ""
	tokens := Tokenize(source, TESTING_SOURCE)

	if len(tokens) != 1 && tokens[0].Kind != EOF {
		t.Fail()
	}
}

func TestBasicMath(t *testing.T) {
	source := "10 + 45 * -45.312 / (-2.1 - -5);"
	tokens := Tokenize(source, TESTING_SOURCE)
	expectedKinds := []TokenKind{
		NUMBER, PLUS, NUMBER, STAR, DASH, NUMBER, SLASH, OPEN_PAREN, DASH, NUMBER, DASH, DASH, NUMBER, CLOSE_PAREN, SEMI_COLON, EOF,
	}

	if len(expectedKinds) != len(tokens) {
		t.Fail()
	}

	for indx, tk := range tokens {
		if tk.Kind != expectedKinds[indx] {
			t.Fail()
		}
	}
}
