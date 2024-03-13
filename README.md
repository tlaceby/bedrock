# Bedrock **(.br)**
Bedrock is a statically typed programming language designed to provide a solid foundation for building general-purpose computer programs. With a syntax inspired by Rust, Go, and C#, Bedrock aims to be simple, concise, and easy to use while offering powerful modern features.

## Key Features

    Tagged Enums: Bedrock supports tagged enums, similar to Rust, allowing you to define custom types with multiple variants. This enables expressive and type-safe coding patterns.
    Errors as Values: Bedrock treats errors as first-class citizens, using the Result type to explicitly handle and propagate errors. This encourages robust error handling and helps prevent unexpected program behavior.
    Option Type: Bedrock provides an Option type to represent the presence or absence of a value, eliminating the need for null references and reducing the chances of null pointer exceptions.
    Packaging System: Bedrock comes with a built-in packaging system, making it easy to organize, share, and reuse code. The packaging system allows for modular development and facilitates the creation of libraries and frameworks.
    Generics: Bedrock supports generic programming, enabling the creation of reusable and type-safe code. Generics allow you to write functions and data structures that can work with multiple types, promoting code reuse and reducing duplication.
    Portability: Bedrock is designed to be portable across different platforms. It compiles to a modern virtual machine, ensuring that Bedrock programs can run consistently across various operating systems and architectures.

## Getting Started

To get started with Bedrock, follow these steps:

    Install the Bedrock compiler and virtual machine by following the installation guide in the documentation.
    Write your Bedrock code using your favorite text editor or IDE.
    Compile your Bedrock code using the Bedrock compiler, which will generate bytecode for the virtual machine.
    Run the compiled bytecode on the Bedrock virtual machine to execute your program.

## Examples

Here's a simple "Hello, World!" program in Bedrock:
```br
# The main function is the entry point to the application.
fn main() {
    println("Hello, World!");
}

```

Bedrock is open-source software licensed under the MIT License.
