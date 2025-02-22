package bytecode;

Opcode :: enum u8 {
    HALT = 0,
    LOAD_CONST // LOAD_CONST CONST_INDX ; loads a constant from the $constants in chunk at address of $0. Pushes value onto stack.
}