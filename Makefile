
CXX=clang++-mp-3.3 -O3 -stdlib=libc++
CXXFLAGS+=-std=c++11 -Iinclude

EXAMPLES=\
  examples/call_traits \
  examples/eliminate \
  examples/immutable \
  examples/meta_list \
  examples/overload \
  examples/unit

.PHONY: all clean

all: $(EXAMPLES:%=%.exe)

include $(EXAMPLES:%=%.d)

%.exe: %.o
	$(CXX) $(LDFLAGS) -o $@ $< $(LDLIBES) $(LDLIBS)

%.d: %.cpp
	$(CXX) -MM $(CPPFLAGS) $(CXXFLAGS) $< | perl -pe 's,^\S+\.o\s*:\s*,$(@:%.d=%.o) $@ : ,g' > $@

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

%.s: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -S -o $@ $<

clean:
	rm -f $(EXAMPLES) $(EXAMPLES:%=%.o) $(EXAMPLES:%=%.s) $(EXAMPLES:%=%.d)

