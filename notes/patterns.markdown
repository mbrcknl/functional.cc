
# Pattern matching design principles

For data structure users:

- easy to use, low overhead.
- powerful.
- natural, intuitive.
- safe.

For data structure implementors:

- reasonably straightforward, moderate overhead.
- data types need not depend on fp library.
- pattern types need not depend on fp library.
  - provide base/utility classes to reduce boilerplate.
- support new pattern schema without modifying data types.
- pattern schema should not define or specialise in the fp namespace.

# Glossary

data type
: a type whose values may be matched against one or more patterns.

data value
: a value of a data type.

pattern type
: a type that represents a pattern that may match a data value,
  whose values capture deconstructed data values.

pattern value
: a value of a pattern type.

pattern schema
: a specification of which pattern types can match a particular data types.

# Patterns vs eliminators

Eliminators allow implicit conversions between Specs and Funcs,
while pattern types must match exactly.

Eliminators give names to the components arising out the match,
while patterns just convert to a matching type with getters for
the components.

