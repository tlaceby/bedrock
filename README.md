# Bedrock **(.br)**

Bedrock is a statically typed programming language designed to provide a solid foundation for building general-purpose computer programs. With a syntax inspired by Rust, Go, and C#, Bedrock aims to be simple, concise, and easy to use while offering powerful modern features.

## Examples

Here's a simple program showing off some features of the language.

```rs
// Imports
mod main; // the entry point must be mod main
import("std/slices") as slices;
import("../lib/foo.br") as foo;

// Variable Declataions & Assignemnts
let foo: Number; // cannot be used until value is assigned
let bar: String = "Hello world";

foo = 10.2;

// foo = "This would cause an error"; -> Invalid Types
const foobar: String = @string(foo) + bar;
const pi = 3.14;

// Conversions & Casts
const angle = pi * 2;
const message: String = "The angle is " + @string(angle);

// Polymorphism
trait Stats {
  fn displayStats () -> Void;
}

struct Person {
  name: String;
  age:  Number;

  // Methods which must be implimented before struct can be created/used.
  fn displayStats () -> Void;
  static fn new (name: String, age: Number) -> Self;
}

impl Person {
  fn displayStats () -> Void {
    println("Age: "   + @string(age));
    println("Speed: " + @string(speed));
  }

  static fn new (name: String, age: Number) -> Self {
    return Person{
      name, age,
    };
  }
}

struct Animal {
  breed: String;
  age:   Number;
  speed: Number;

  fn displayStats () -> Void;
}

impl Animal {
  fn displayStats () -> Void {
    println("Breed: " + breed);
    println("Age: "   + @string(age));
    println("Speed: " + @string(speed));
  }
}

let thingsWithStats: []Stats{
  Person::new("Tyler L", 24),
  Animal{
    breed: "Dog",
    age: 10,
    speed: 4.3,
  }
};

for value in thingsWithStats {
  value.displayStats();

  // How to access the underlying type.
  match value {
    .Person {
      println("It'a a Person{}");
    },
    .Animal {
      println("It's a Animal{}");
    }
  }
}

// Functions

fn add (x: Number, y: Number) -> Number {
  return x + y;
}

// Variadic function. Expects atleast 2 arguments
fn min (a: Number, b: Number, dyn args: []Number) -> Number {
  let minFound: Number = a;

  if b < a {
    minFound = b;
  }

  for el in args {
    if el < minFound {
      minFound = el;
    }
  }

  return minFound;
}

add(10, 20); // 30;
min(1, -25); // -25
min(1, 2, 3, 4, 5, 0); // 0

// Slices

let numbers: []Number = []Number{ 1, 2, 3, 4 };
numbers = slices::append(numbers, 10); // slices are fixed in size. No resizing without build in methods
numbers[4] = 5;

println(@string(numbers)); // [1, 2, 3, 4, 5]

// Constant size slice. No resizing
const BUFFER_SIZE = 1024;
const buffer = [BUFFER_SIZE]Number{};

slices::fill(buffer, 1);

if slices::every(buffer, 1) {
  println("Every value is a 1");
} else {
  panic("Something went wrong. Slice should have all 1's");
}

// Will set each element in the buffer to it's index [0, 1, ... 1023]
slices::map(buffer, fn (indx: Number, _: Number) -> Number {
  return indx;
});

// Errors (IN-PROGRESS)

// Assert will panic if the size is not met
@assert(len(buffer), BUFFER_SIZE);

// assert_or lets you handle errors with a callback
@assert_or(len(buffer), BUFFER_SIZE, fn () {
  println("Buffer not the proper size");
});


// Unsafe Actions
unsafe {
  const moreThingsWithStats: []Stats{
    Person::new("Tyler L", 24),
    Animal{
      breed: "Dog",
      age: 10,
      speed: 4.3,
    }
  };

  // You can only perform an unsafe_cast inside a unsafe scope
  let person = @unsafe_cast(Person, moreThingsWithStats[0]);
  let animal = @unsafe_cast(Animal, moreThingsWithStats[1]);

  person.name = "John Doe";
  animal.speed = 1.2;
}

```

Bedrock is open-source software licensed under the MIT License.
