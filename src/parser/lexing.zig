const std = @import("std");

var gpa = std.heap.GeneralPurposeAllocator(.{}){};
const allocator = gpa.allocator();

pub const TkTag = enum {
    eof,
    number,
    str,
    symbol,

    // Positional & Grouping
    lparen,
    rparen,
    lcurly,
    rcurly,
    lbracket,
    rbracket,
    semicolon,
    colon,
    comma,
    period,

    // Arithmetic
    plus,
    dash,
    star,
    slash,
    percent,

    // Comparison | Logical
    lt,
    lte,
    gt,
    gte,
    eq,
    neq,
    _or,
    _and,

    // Keywords
    import,
    _pub,
    let,
    mut,
    self,
    _fn,
    _return,
    _while,
    _for,
    match,
    _if,
    _else,
    _enum,
    _type,
    _struct,
    impl,
};

pub const TkPos = struct {
    line: usize,
    offset: usize,
};

pub const Tk = struct {
    tag: TkTag,
    contents: []const u8,
    pos: TkPos,
};

pub const BedrockFile = struct {
    path: []const u8,
    filename: []const u8,

    pub fn from_relative(rel_path: []const u8) !BedrockFile {
        const cwd_path = try std.fs.cwd().realpathAlloc(allocator, ".");
        defer allocator.free(cwd_path);
        const abs_path = try std.fs.path.resolve(allocator, &.{
            cwd_path,
            rel_path,
        });

        const filename = "";
        return BedrockFile{ .path = abs_path, .filename = filename };
    }
};

pub const Lexer = struct {
    buffer: []const u8,
    filepath: []const u8,
    pos: TkPos,

    pub fn from_relative_path(rel_path: []const u8) !Lexer {
        const cwd_path = try std.fs.cwd().realpathAlloc(allocator, ".");
        defer allocator.free(cwd_path);
        const abs_path = try std.fs.path.resolve(allocator, &.{
            cwd_path,
            rel_path,
        });

        return Lexer.init(abs_path);
    }

    pub fn init(filepath: []const u8) !Lexer {
        const file = try std.fs.openFileAbsolute(filepath, .{ .mode = .read_only });
        defer file.close();
        const info = try file.stat();
        const buffer = try allocator.alloc(u8, info.size);
        const bytesRead = try file.readAll(buffer);

        if (bytesRead != info.size) {
            std.debug.panic("Could not read the entire file. Expected to read {} bytes but failed after {}.", .{ bytesRead, info.size });
        }

        return Lexer{
            .pos = TkPos{ .line = 0, .offset = 0 },
            .filepath = filepath,
            .buffer = buffer,
        };
    }
};
