
# Masala

A library for functional programming in C++, with support for efficient
immutable algebraic data types, pattern matching, and other goodies.

## Requirements

Masala is a [C++11][] library, so you'll need a recent C++ compiler, building
in C++11 mode. I am using [GCC][] [4.7.2][4.7], with `g++ -std=c++11`. Recent
versions of [Clang][] should probably work, though I haven't tried this.

There are no library dependencies other than the C++11 standard library.

[C++11]: http://en.wikipedia.org/wiki/C++11
[GCC]: http://gcc.gnu.org/
[4.7]: http://gcc.gnu.org/gcc-4.7/
[Clang]: http://clang.llvm.org/

Currently, masala is an include-only library, so you just need to ensure your
compiler can find the files in the `include` directory.

## License

Copyright Matthew Brecknell 2012. Distributed under the Boost Software License,
Version 1.0. (See accompanying file `LICENSE.txt` or a copy at [boost.org][].)

[boost.org]: http://www.boost.org/LICENSE_1_0.txt

