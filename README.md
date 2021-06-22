# This is Scanner++ Library

The idea of this project is to have (Java) Scanner functionalities on C++.
This is an experimental library in Header Only format (just add `Scanner.hpp` to your project and that's it!).

## Build with Conan + Bazel

This project tries to use both Conan and Bazel (during tests and benchmarks, not production).
By Conan, the files at `conandeps` are created in a compatible manner with Bazel.

### Dependencies

`conan search benchmark -r conan-center`

`conan search tl-expected -r conan-center`

### Conan + Bazel

```
pip install conan
```

Google benchmark requires `D_GLIBCXX_USE_CXX11_ABI` [see issue](https://github.com/google/benchmark/issues/897):

```
conan install . -s compiler.libcxx=libstdc++11
```

or install in local `conan_cache` folder:

``` 
mkdir -p conan_cache && export CONAN_USER_HOME=${PWD}/conan_cache && conan install . -s compiler.libcxx=libstdc++11
```


At `conandeps/benchmark/BUILD`, small fix, add these into `cc_library`:

```
    deps = [":benchmark_precompiled"],
    linkopts = ["-lpthread"],
```

This prevents link issue with `-lbenchmark` (`libbenchmark.a`).

```
cd tests
./run_bench.sh
```

If this doesn't work, `make bench2` could be manually executed, such as:

```
g++-9 -g -D_GLIBCXX_USE_CXX11_ABI=0 --std=c++17 -DNDEBUG bench.cpp -I/home/imcoelho/.conan/data/tl-expected/20190710/_/_/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9/include -I/home/imcoelho/.conan/data/benchmark/1.5.3/_/_/package/9ec7e6c55ee108e231bdef75e23776b8b86d821d/include -L/home/imcoelho/.conan/data/benchmark/1.5.3/_/_/package/9ec7e6c55ee108e231bdef75e23776b8b86d821d/lib -lbenchmark -pthread 
```

### Listing dependencies

```
bazel query 'labels(hdrs, @tl-expected//...)'
bazel query 'labels(hdrs, @benchmark//...)'
bazel query 'labels(hdrs, ...)'
```

## Current and Legacy versions

Current version is `1.0`, located on `./src` folder, and licensed `MIT`.

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

### Example 1 (default separators)
```c++
#include <scannerpp/Scanner.hpp>

using namespace scannerpp;

int main() {
    // (1.a) Hello World
    Scanner scanner("Hello World");
    std::cout << scanner.next(); // returns "Hello"
    std::cout << scanner.next(); // returns "World"
    //
    // (1.b) Hello World
    Scanner scanner2("Hello World");
    while(scanner2.hasNext())
        std::cout << scanner2.next() << std::endl; // "Hello\nWorld"
    //
    // (1.c) Hello World
    Scanner scanner3("Hello World");
    std::string s;
    while ((s = scanner3.next()) != "")
        std::cout << s << std::endl; // "Hello\nWorld"
    //
    return 0;
}
```

### Example 2 (other separators)
```c++
#include <scannerpp/Scanner.hpp>

using namespace scannerpp;

int main() {
    Scanner scanner("Hello;World");
    scanner.useSeparators(" ;\n\t"); // using semi-colon as separator too
    std::cout << scanner.next(); // returns "Hello"
    std::cout << scanner.next(); // returns "World"
    //
    return 0;
}
```


### Example 3 (read from File or cin)
```c++
#include <scannerpp/Scanner.hpp>

using namespace scannerpp;

int main() {
    Scanner scanner(File("myfile.txt"));
    // 'myfile.txt' contains "Hello World" inside it
    std::cout << scanner.next(); // returns "Hello"
    std::cout << scanner.next(); // returns "World"
    Scanner scanner2(&cin); // passing 'cin' as pointer
    std::cout << scanner2.next() << std::endl; // blocks until user inputs data
    //
    return 0;
}
```

### Example 4 (reading numbers)
```c++
#include <scannerpp/Scanner.hpp>

using namespace scannerpp;

int main() {
    Scanner scanner("-1 2147483647    2147483648    8589934592    3.14  8");
    // remember that 'nextInt()' returns std::optional, to prevent exceptions
    auto x1 = *scanner.nextInt();    // -1 (int)
    auto x2 = *scanner.nextInt();    // 2147483647 (int)  (no overflow)
    scanner.hasNextInt();            // -------> FALSE!
    auto x3 =  scanner.nextInt();    // this is 'nullopt' (overflow!)
    auto x4 = *scanner.nextLong();   // this is 8589934592 (long)
    auto x5 = *scanner.nextFloat();  // this is 3.14 (float)
    auto x6 = *scanner.nextDouble(); // this is 8.0 (double)
    printf("%d %d %s %ld %f %f\n", x1, x2, (x3==std::nullopt?"nullopt":""), x4, x5, x6);
    // outputs: -1 2147483647 nullopt 8589934592 3.140000 8.000000
    return 0;
}
```


### Example 5 (reading numbers with c++17 if)
```c++
#include <scannerpp/Scanner.hpp>

using namespace scannerpp;

int main() {
    Scanner scanner("2147483647  2147483648");
    // Note that a c++17 'if' can efficiently declare and consume variables,
    //   as this completely avoids the need for 'hasNextInt' verification
    if(auto x2 = scanner.nextInt(); x2)
       printf("x2 is int = %d\n", *x2); // will print
    if(auto x3 = scanner.nextInt(); x3)
       printf("x3 is int = %d\n", *x3); // will not print
    //
    return 0;
}
```


## How to Install

### header-only copy
This is a header-only library.
To keep it simple, just copy `src/Scanner.hpp` to your project and have fun!
This way, you can just add `#include "Scanner.hpp"`.

If you want to have it in your system, e.g. to use as `#include <scannerpp/Scanner.hpp>`, you need to do the following steps (next section).

### System Installation
First, check if it passes all tests: `make test` and `make test-coverage`.

After, just type `make install` (this will require `sudo`).
File will be installed on `/usr/local/include/scannerpp`.

After that, `make test-install`, should return:
```
g++ --std=c++17 test-install.cpp -o scannerpp_test_install
./scannerpp_test_install
Hello
World
```

## License

MIT License

Copyleft, OptFrame group

2008-2020
