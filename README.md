# This is Scanner++ Library

The idea of this project is to have (Java) Scanner functionalities on C++.


## Current and Legacy versions

Current version is `1.0-alpha`, located on `./src` folder, and licensed `MIT`.

Note that previous version `0.98-alpha`, located on `./legacy`, was licensed `LGPLv3` (project previously ported from SourceForge: https://sourceforge.net/projects/scannerpp/).


There are many reasons for the change:

- A complete code rewrite was necessary, as `0.98-alpha` version did not consider important `c++11` features, such as move semantics, nor any other recent c++ advances (`c++14`, `c++17` and `c++20`)
- The organization as a **single header file** (`.hpp`) is much more desirable than separated `.h` and `.cpp`, as a link library format. This facilitates integration with projects.
- Unit testing was heavily desired, but would require some breaking changes. Now it's available on `./tests` folder.
- `MIT` license is more desirable than `LGPLv3`, since this project is intended for broad usage, so projects should not have to care about license on practice (which is not the case for `LGPLv3`)

For these reasons, code was rewritten with tests and new license, and `legacy` version will not be improved (it is considered `deprecated`).

### An exception-less design

Exceptions are now deprecated, so every `next()` operation will return an `optional` output. This effectively removes the necessity of a `hasNext()` method, which we will still support if one prefers this kind of logic (note that output will still be `optional`, requiring `operator*`).

## How to use

Example 1 (default separators):
```c++
#include "Scanner.hpp"

using namespace scannerpp;

int main() {
    // (1.a) Hello World
    Scanner scanner("Hello World");
    std::cout << *scanner.next(); // returns "Hello"
    std::cout << *scanner.next(); // returns "World"

    // (1.b) Hello World
    Scanner scanner2("Hello World");
    while(scanner2.hasNext())
        std::cout << *scanner2.next() << std::endl; // "Hello\nWorld"

    // (1.c) Hello World
    Scanner scanner3("Hello World");
    std::optional<std::string> v;
    while(v = scanner3.next()) // no exception and no double-seek!
        std::cout << *v << std::endl; // "Hello\nWorld"

    return 0;
}
```

Example 2 (other separators):
```c++
#include "Scanner.hpp"

using namespace scannerpp;

int main() {
    Scanner scanner("Hello;World");
    scanner.useSeparators(" ;\n\t"); // using semi-colon as separator too
    std::cout << *scanner.next(); // returns "Hello"
    std::cout << *scanner.next(); // returns "World"
    return 0;
}
```


MIT License

Copyleft, OptFrame group

2008-2020
