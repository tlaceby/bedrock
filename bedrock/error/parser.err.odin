package error;

import "core:fmt"

ParserErrKind :: enum {}

ParserError :: struct {
    kind: ParserErrKind,
    message: string,
    location: Location,
}

parser_error :: proc(kind: ParserErrKind, message: string, location: Location = Location{}) -> ParserError {
    return ParserError{
        kind = kind, 
        message = message,
        location = location,
    }
}

@(private)
display_parser_error:: proc(err: ^ParserError) {
    kind_str := fmt.aprintf("%v", err.kind)
    defer delete(kind_str)
    header  := create_error_header(kind_str, err.location, err.message)
    preview := create_error_preview(err.location)
    
    defer delete(header)
    defer delete(preview)
    fmt.print(header)
    fmt.print(preview)
}