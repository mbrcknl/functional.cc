
A library for functional programming in C++, with support for efficient
immutable algebraic data types, pattern matching, and other goodies.

Experimental, and unfinished. Do not use.

## Requirements

This is a [C++11][] library, so you'll need a recent C++ compiler, building in
C++11 mode. I am using [Clang][] 3.3, with these options:

    clang++ -std=c++11 -stdlib=libc++

There are no library dependencies other than the C++11 standard library.

[C++11]: http://en.wikipedia.org/wiki/C++11
[Clang]: http://clang.llvm.org/

Currently, this is an include-only library, so you just need to ensure that the
`include` directory is in the `#include <...>` search path.

## License

Copyright Matthew Brecknell 2013. Distributed under the Boost Software License,
Version 1.0. (See accompanying file `LICENSE.txt` or a copy at [boost.org][].)

[boost.org]: http://www.boost.org/LICENSE_1_0.txt

