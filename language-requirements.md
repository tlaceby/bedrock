## Language Requirements

- The language will have a two pass typechecker.
    - The first pass will be able to handle definining Function declarations, struct declarations, type decarations, imports, etc... This will also allow files inside a module to use other files in the same module within a **cycle**.

- Runtime will be a bytecode virtual machine of a custom type.
- First class support for impl/method overlading will be supported at the package level.
    - If the foo module defined a type called `Foo`, this type can define impl methods, overloads and such for this method. 
    - This module can determine whether this type is public or private to other modules. 
    - This module can also allow/dissalow additonal methods/overloads to be added by other modules if they want.

## Type System

- Number
- String
- Boolean
- Result
- Option
- Slice
- Map