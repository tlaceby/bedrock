package error;

import "core:fmt"


LexicalErrKind :: enum{
    UnexpectedEOF,
    UnknownToken,
}

LexicalError :: struct {
    kind: LexicalErrKind,
    message: string,
    location: Location,
}

lexical_error :: proc(kind: LexicalErrKind, message: string, location: Location = Location{}) -> LexicalError {
    return LexicalError{
        kind = kind,
        message = message,
        location = location,
    }
}

@(private)
display_lexical_error:: proc(err: ^LexicalError) {
    kind_str := fmt.aprintf("%v", err.kind)
    defer delete(kind_str)
    header  := create_error_header(kind_str, err.location, err.message)
    preview := create_error_preview(err.location)

    defer delete(header)
    defer delete(preview)
    fmt.print(header)
    fmt.print(preview)
}