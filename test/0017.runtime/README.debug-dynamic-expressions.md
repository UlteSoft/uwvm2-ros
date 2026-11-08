# Dynamic source-reference expression parsing

`parse_source_expression` uses an explicit continuation vector for parentheses,
prefix dereferences and the read-only Go `len`/`cap` grammar. Default admission is
bounded by the supplied input and available storage. It has no fixed text,
selector-count or nesting ceiling. A caller may still supply an explicit byte,
step or nesting budget through `source_expression_limits`.

Each continuation consumes a checked input token and is removed once during
unwinding. Nested expressions do not recurse through the native call stack.
Postfix precedence is preserved: `*p[1]` applies the index before dereferencing,
while `(*p)[1]` dereferences before indexing. The scalar parser's primary entry
consumes only its own primary, leaving subsequent scalar operators to its owner.
Parsing remains read-only syntax processing; it provides no guest-memory or
native-address authority.

The maintained `debug_source_expression_dynamic.cc` fixture checks 131,072
levels of parentheses and dereferences, 32,768 selector operations, a root name
longer than 64 KiB, malformed deep input, exact explicit budgets and rejected
calls, assignments and nested Go builtins. The existing
`debug_source_dwarf_expression.cc` retains precedence, malformed-input and
copied-pointer planning checks.

Qualification includes all five ROS Linux targets, native AddressSanitizer and
UndefinedBehaviorSanitizer, a native execution with a 512 KiB stack, and a
40,045-case comparison against the preceding parser with the same explicit
legacy-sized budgets. Source scalar arithmetic, Go builtins and Rust/Zig
postfix users are tested separately because they use the internal primary
and postfix entry points. The allocation-failure fixture injects 64 consecutive
allocation failures on every target, verifies cleared output after each failure,
and then successfully parses the complete expression. Native sanitizer runs
also check cleanup during these failures.

This scope is the source-reference syntax component. Controller transport,
scalar evaluator, copied-object traversal and IDE input budgets have separate
implementations and qualification. A successful parse does not bypass their
stop, ownership, generation or complete-range validation.
