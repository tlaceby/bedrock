package main

import "core:io"
import "core:strings"
import "core:fmt"
import "core:os"
import "lexer"
import "error"

main :: proc () {
    tokens, err := lexer.tokenize("test.br")
    defer delete(tokens)

    if err != nil { 
        error.display_error(&err)
        os.exit(1)
    }

    fmt.printf("Tokens %d\n%#v\n", len(tokens), tokens)
}

