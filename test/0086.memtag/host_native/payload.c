/* Instrumented separately from the observer to test native compiler/loader MTE. */
extern void observe_pointer(char const*, void*);
extern void consume_pointer(void*);
char host_mte_global[37];
__attribute__((noinline)) void host_mte_global_probe(void) {
    host_mte_global[0] = 17;
    observe_pointer("global", host_mte_global);
}
__attribute__((noinline)) void host_mte_stack_probe(void) {
    char local[37];
    local[0] = 23;
    consume_pointer(local);
    observe_pointer("stack", local);
}
