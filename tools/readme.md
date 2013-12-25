## Cppcheck developer and build tools

### matchcompiler.py

The matchcompiler.py script performs a few code transformations to cpp files under the lib/ directory. These transformations are related to the use of `Token::Match()` function and are intented to improve code performance. The transformed files are saved on the build/ directory.

For example, suppose there is a file example.cpp under lib/:
```cpp
// lib/example.cpp
void f1() {
    Token::Match(tok, "abc");
}

void f2() {
    const char *abc = "abc";
    Token::Match(tok, abc);
}
```
We run matchcompiler.py:
```shell
$ python tools/matchcompiler.py
```
A file example.cpp will be generated on the build/ directory:
```cpp
// build/example.cpp
#include "token.h"
#include "errorlogger.h"
#include <string>
#include <cstring>
static const std::string matchStr1("abc");
// pattern: abc
static bool match1(const Token* tok) {
    if (!tok || !(tok->str()==matchStr1)/* abc */)
        return false;
    return true;
}
void f1() {
    match1(tok);
}

void f2() {
    const char *abc = "abc";
    Token::Match(tok, abc);
}
```
The usage of `Token::Match()` in `f1()` has been optimized, whereas the one in `f2()` couldn't be optimized.
To compile the transformed code instead of the lib/ code use the `SRCDIR` makefile variable:
```shell
$ make SRCDIR=build
```
### extracttests.py

### dmake.cpp

### reduce.cpp

### times.sh
