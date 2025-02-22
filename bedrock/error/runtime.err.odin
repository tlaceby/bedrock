package error;

import "core:fmt"

RuntimeErrorKind :: enum {}

RuntimeError :: struct {
    kind: RuntimeErrorKind,
    message: string,
    location: Location,
}


runtime_error :: proc(kind: RuntimeErrorKind, message: string, location: Location = Location{}) -> RuntimeError {
    return RuntimeError{
        kind = kind, 
        message = message,
        location = location,
    }
}


@(private)
display_runtime_error:: proc(err: ^RuntimeError) {
    kind_str := fmt.aprintf("%v", err.kind)
    defer delete(kind_str)
    header  := create_error_header(kind_str, err.location, err.message)
    preview := create_error_preview(err.location)
    
    defer delete(header)
    defer delete(preview)
    fmt.print(header)
    fmt.print(preview)
}