const std = @import("std");
const lexing = @import("parser/lexing.zig");

pub fn main() !void {
    _ = try lexing.Lexer.from_relative_path("examples/syntax.br");
}
