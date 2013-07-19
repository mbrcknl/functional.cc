
CXX=clang++-mp-3.3
CXXFLAGS+=-std=c++11 -stdlib=libc++ -Iinclude

EXAMPLES=\
  examples/call_traits \
  examples/eliminate \
  examples/immutable \
  examples/meta_list \
  examples/overload \
  examples/unit

.PHONY: all

all: $(EXAMPLES)

