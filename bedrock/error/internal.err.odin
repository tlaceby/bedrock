package error;
import "core:fmt"

InternalErrKind :: enum {
    ProgramUsage,
    OutOfMemory,
}

InternalError :: struct {
    kind: InternalErrKind,
    message: string,
    location: Location,
}


internal_error :: proc(kind: InternalErrKind, message: string, location: Location = Location{}) -> InternalError {
    return InternalError{
        kind = kind,
        message = message,
        location = location,
    }
}

@(private)
display_internal_error:: proc(err: ^InternalError) {
    kind_str := fmt.aprintf("%v", err.kind)
    defer delete(kind_str)
    header  := create_error_header(kind_str, err.location, err.message)
    preview := create_error_preview(err.location)
    
    defer delete(header)
    defer delete(preview)
    fmt.print(header)
    fmt.print(preview)
}