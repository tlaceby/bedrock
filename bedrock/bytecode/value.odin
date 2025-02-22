package bytecode;

Value :: union {
    NumberVal,
    // StringVal,
    // BooleanVal,
}

NumberVal :: struct {
    value: f64,
}

StringVal :: struct {
    value: string,
}

BooleanVal :: struct {
    value: bool,
}