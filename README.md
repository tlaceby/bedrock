# Bedrock **(.br)**

Bedrock is a statically typed programming language designed to provide a solid foundation for building general-purpose computer programs. With a syntax inspired by Rust, Go, and C#, Bedrock aims to be simple, concise, and easy to use while offering powerful modern features.

## Examples

Here's a simple program showing off some features of the language.

```rs
// main.br

fn combine <T> (a: T, b: T) -> T {
  return a + b;
}

struct Circle {
  radius: number;

  static fn new (r: number) -> Circle {
    return Circle {
      radius: r,
    };
  }

  fn setRadius (r: number) -> Circle {
    self.radius = r;
  }
}

let message = combine <string>("Hello ", "world");
message = "Hello ";

const foo = message + "world";
```

Bedrock is open-source software licensed under the MIT License.
