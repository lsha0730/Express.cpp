## Naming Conventions

> Based on the C++ Core Guidelines by Bjarne Stroustrup and Herb Sutter

### Classes

- **Class name**: PascalCase
- **Class variables** (private): snake_case\_ (with '\_' to denote it being a member variable)
- **Class variables** (public): snake_case
- **Getters**: no 'get' prefix, just the name of the variable being fetched (Ex. `length()`)
- **Setters**: 'set' prefix, camelCase (Ex. `setLength()`)
- **Placement for public/protected/private**: The more private it is, the less important its visibility. Declare public -> protected -> private fields in top->down order for headers and implementations

### Structure

- **File names**: snake_case, `.cpp` for cpp files, `.h` for header files
- **Module names**: PascalCase_h
- **Namespace names**: snake_case
