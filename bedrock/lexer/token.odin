#+feature dynamic-literals
package lexer;


Location :: struct {
    filepath: string,
    line: uint,
    offset: uint,
}

Token :: struct {
    kind: TokenKind,
    value: string,
    pos: Location,
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
}

@(private)
keyword_lu := map[string]TokenKind{
    "mod" = .Mod,
    "use" = .Use,
    "const" = .Const,
    "let" = .Let,
    "dyn" = .Dyn,
    "for" = .For,
    "while" = .While,
    "continue" = .Continue,
    "break" = .Break,
    "return" = .Return,
    "fn" = .Fn,
    "match" = .Match,
    "trait" = .Trait,
    "impl" = .Impl,
    "override" = .Override,
    "pub" = .Pub,
    "from" = .From,
    "in" = .In,
}