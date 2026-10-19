/* Compile separately with -g -O0 -fno-lto. No Host import or Guest trap. */
#include "../../debug/include/uwvm_language_snapshot.h"
__attribute__((noinline, optnone, used))
void uwvm_python_debug_anchor(uwvm_language_snapshot const *current) {
    /* Actual source breakpoint and actual pointer parameter DWARF location. */
    volatile uint64_t current_sequence = current->header.sequence;
    (void)current_sequence;
}
