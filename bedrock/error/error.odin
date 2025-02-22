package error;

import "core:strings"
import "base:runtime"
import "core:os"
import "core:fmt"
import "core:mem"

BedrockError :: union {
    InternalError,
    LexicalError,
    ParserError,
    TypeCheckError,
    RuntimeError,
}

Location :: struct {
    filepath: string,
    line: uint,
    offset: uint,
}


make_location :: proc(filepath: string, line: uint, offset: uint) -> Location {
    return Location{
        filepath = filepath,
        line = line,
        offset = offset,
    }
}

@(private)
create_error_header :: proc (kind_str: string, loc: Location, err_message: string) -> string {
    header := fmt.aprintfln("%s::%s(%d:%d)", kind_str, loc.filepath, int(loc.line), int(loc.offset))
    message := fmt.aprintfln("%s on line %d\n", err_message, int(loc.line))
    defer delete(header)
    defer delete(message)

    return strings.concatenate([]string{header, message})
}

@(private)
create_error_preview :: proc (loc: Location) -> string {
    return ""
}


display_error :: proc (error: ^BedrockError) {
    switch &err in error {
        case InternalError: display_internal_error(&err)
        case LexicalError: display_lexical_error(&err)
        case ParserError: display_parser_error(&err)
        case TypeCheckError: display_typecheck_error(&err)
        case RuntimeError: display_runtime_error(&err)
    }
} 