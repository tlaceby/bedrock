# Bedrock **(.br)**

Bedrock is a statically typed programming language designed to provide a solid foundation for building general-purpose computer programs. With a syntax inspired by Rust, Go, and C#, Bedrock aims to be simple, concise, and easy to use while offering powerful modern features.

## Examples

Here's a simple program showing off some features of the language.

```rs
// main.br
fn combine <T> (a: T, b: T) -> T {
  return a + b;
}

struct Combinable<T> {
  data: T;

  fn setData(data: T) -> Self {
    self.data = data;
    return self;
  }

  fn combine(other: T) -> T {
    self.data = self.data + other;
    return self.data;
  }

  static fn new(init: T) -> Self {
    return Self {
      data: init,
    };
  }
}

fn main () {
  const c1 = Combinable<number>::new(10);
  const c2 = Combinable<string>{
    data: "Hello ",
  };

  c2.combine("world");

  const names = []string{"john", "doe"};
  const numbers = []number{1, 2, 3, 4, 2, 3, 4};

  const john = names[0];
  const num = numbers[2];

  const sum = combine<number>(numbers[0], numbers[1]);
  const name = combine<string>(names[0], names[1]);

  let i = 0;
  while (i lt 10) {
    i += 1;
  }
}
```

Bedrock is open-source software licensed under the MIT License.
