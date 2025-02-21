package token;

Position :: struct {
    start: uint,
    end: uint,
}

TokenKind :: enum {
    EndOfFile,
    Idenfifier,
    Number,
    StringLiteral,

    OpenCurly,
    CloseCurly,
    OpenBrace,
    CloseBrace,
    OpenBracket,
    CloseBracket,

    Semicolon,
    Colon,
    DblColon,
    Comma,
    Dot,
    DotDot,
    Question,
    At,
    Ampersand,
    Dollar,

    Star,
    Slash,
    Plus,
    Sash,
    Percent,

    Or,
    And,

    Assignment,
    Equals,

    Mod,
    Use,
    Const,
    Let,
    Dyn,
    For,
    While,
    Continue,
    Break,
    Return,
    Fn,
    Match,
    Trait,
    Impl,
    Override,
    Pub,
    From,
    In,
    Range,
}

Token :: struct {
    kind: TokenKind,
    value: string,
    pos: Position,
}