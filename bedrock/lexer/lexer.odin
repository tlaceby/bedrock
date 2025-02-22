package lexer;
import error "../error"
import "core:fmt"

@(private)
Lexer :: struct {
    tokens: [dynamic]Token,
    location: Location,
}

@(private)
create_lexer :: proc(filepath: string) -> (Lexer, error.BedrockError) {
    tokens, err  := make_dynamic_array([dynamic]Token)
    if err != nil {
        return Lexer{}, nil
    }

    lex := Lexer{
        tokens = tokens,
        location = Location{
            offset  = 0,
            line = 1,
            filepath = filepath,
        }
    }

    return lex, nil
}

// Given a filepath, will tokenize the file into bedrock tokens. Returns all errors which were found.
tokenize :: proc (filepath: string) -> (tokens: [dynamic]Token, err: error.BedrockError) {
    lex : Lexer
    lex, err = create_lexer(filepath)

    if err != nil {
        return lex.tokens, err
    }

    return tokens, err
}


make_location :: proc(filepath: string, line: uint, offset: uint) -> Location {
    return Location{
        filepath = filepath,
        line = line,
        offset = offset,
    }
}