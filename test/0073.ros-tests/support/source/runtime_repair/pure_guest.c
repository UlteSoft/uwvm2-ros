/**
 * @file pure_guest.c
 * @brief Exercise a deterministic guest without any host imports.
 * @details Preserve real arithmetic in the compiled module, and trap if its
 * observable result differs. Fresh VM processes use the same module bytes.
 */
static volatile unsigned result;

__attribute__((noinline)) unsigned sum(unsigned count)
{
    unsigned value = 0;
    for(unsigned index = 0; index != count; ++index) { value += index; }
    return value;
}

void _start(void)
{
    result = sum(1000);
    if(result != 499500u) { __builtin_trap(); }
}
