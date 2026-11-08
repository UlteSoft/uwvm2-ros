/* Native AArch64/Linux provider probe. Deliberate bad accesses stay in asm.
 * The observer is not instrumented; this is not a fully instrumented VM. */
#define _GNU_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <sys/auxv.h>
#include <sys/mman.h>
#include <sys/prctl.h>
#include <sys/resource.h>
#include <unistd.h>
#include <ucontext.h>
#include <dlfcn.h>
#ifndef HWCAP2_MTE
#define HWCAP2_MTE (1UL << 18)
#endif
#ifndef PROT_MTE
#define PROT_MTE 0x20
#endif
extern void host_mte_global_probe(void);
extern void host_mte_stack_probe(void);
static char const* action;
static int thread_heap;
static uintptr_t logical_tag(uintptr_t pointer) { return (pointer >> 56) & 15; }
static uintptr_t allocation_pointer(uintptr_t pointer) {
    uintptr_t result;
    __asm__ volatile(".arch_extension memtag\nldg %0, [%1]"
                     : "=r"(result) : "r"(pointer) : "memory");
    return result;
}
static unsigned byte_read(uintptr_t pointer) {
    unsigned result;
    __asm__ volatile("ldrb %w0, [%1]" : "=r"(result) : "r"(pointer) : "memory");
    return result;
}
static void byte_write(uintptr_t pointer, unsigned value) {
    __asm__ volatile("strb %w0, [%1]" :: "r"(value), "r"(pointer) : "memory");
}
static void print_hex_signal(char const* label, size_t length, uintptr_t value) {
    char hex[] = "0x0000000000000000\n";
    for (unsigned i = 0; i < 16; ++i)
        hex[2+i] = "0123456789abcdef"[(value >> ((15-i)*4)) & 15];
    (void)write(1, label, length);
    (void)write(1, hex, sizeof hex - 1);
}
static void fault(int number, siginfo_t* info, void* context) {
    (void)number;
    print_hex_signal("SIGNAL_CODE=", 12, (uintptr_t)info->si_code);
    print_hex_signal("FAULT_PC=", 9, ((ucontext_t*)context)->uc_mcontext.pc);
    print_hex_signal("FAULT_ADDRESS=", 14, (uintptr_t)info->si_addr);
    if (info->si_code == 9)
        print_hex_signal("FAULT_ALLOCATION_TAG=", 21,
                         logical_tag(allocation_pointer((uintptr_t)info->si_addr)));
    _Exit(info->si_code == 9 ? 109 : 110);
}
void observe_pointer(char const* kind, void* pointer) {
    uintptr_t raw = (uintptr_t)pointer;
    printf("POINTER %s logical=%lu physical=%lu value=%u\n", kind,
           (unsigned long)logical_tag(raw),
           (unsigned long)logical_tag(allocation_pointer(raw)), byte_read(raw));
    if (!strcmp(action, "bad-read") || !strcmp(action, "bad-write")) {
        uintptr_t bad = (raw & UINT64_C(0x00ffffffffffffff)) | ((logical_tag(raw)^1) << 56);
        puts("BAD_ACCESS_BEGIN");
        if (!strcmp(action, "bad-read")) (void)byte_read(bad);
        else byte_write(bad, 99);
        puts("BAD_ACCESS_NOT_BLOCKED");
    }
}
void consume_pointer(void* pointer) { __asm__ volatile("" :: "r"(pointer) : "memory"); }
static void heap_probe(void) {
    void* pointer = malloc(37);
    if (!pointer) _Exit(41);
    byte_write((uintptr_t)pointer, 42);
    observe_pointer("heap", pointer);
    uintptr_t saved_address = (uintptr_t)pointer;
    puts("FREE_BEGIN");
    free(pointer);
    puts("FREE_OK");
    if (!strcmp(action, "uaf")) {
        puts("UAF_BEGIN"); byte_write(saved_address, 99); puts("UAF_NOT_BLOCKED");
    } else if (!strcmp(action, "double-free")) {
        puts("DOUBLE_FREE_BEGIN");
        /* Keep the second call observable without C optimizer UB folding. */
        void (*volatile release)(void*) = free;
        release((void*)saved_address);
        puts("DOUBLE_FREE_NOT_BLOCKED");
    }
}
static void* worker(void* ignored) {
    (void)ignored;
    printf("THREAD_MODE=%ld\n", prctl(56, 0, 0, 0, 0));
    if (thread_heap) heap_probe(); else host_mte_stack_probe();
    return 0;
}
static int lifecycle(void) {
    void* p = calloc(37, 1);
    if (!p) return 50;
    for (unsigned i = 0; i < 37; ++i) if (byte_read((uintptr_t)p+i)) return 51;
    byte_write((uintptr_t)p, 42);
    void* grown = realloc(p, 1024);
    if (!grown || byte_read((uintptr_t)grown) != 42) return 52;
    p = realloc(grown, 17);
    if (!p || byte_read((uintptr_t)p) != 42) return 53;
    free(p);
    p = 0;
    if (posix_memalign(&p, 64, 128) || !p || ((uintptr_t)p & 63)) return 54;
    byte_write((uintptr_t)p+127, 19);
    free(p);
    puts("LIFECYCLE_OK");
    return 0;
}
int main(int argc, char** argv) {
    struct rlimit no_core = {0, 0};
    (void)setrlimit(RLIMIT_CORE, &no_core);
    setbuf(stdout, 0);
    struct sigaction handler = {0};
    handler.sa_sigaction = fault; handler.sa_flags = SA_SIGINFO;
    sigemptyset(&handler.sa_mask); sigaction(SIGSEGV, &handler, 0);
    printf("START HWCAP2=%lu MODE=%ld\n", getauxval(AT_HWCAP2), prctl(56, 0, 0, 0, 0));
    Dl_info image;
    if (dladdr((void*)free, &image)) printf("LIBC base=%p free=%p\n", image.dli_fbase, (void*)free);
    if (!(getauxval(AT_HWCAP2)&HWCAP2_MTE)) return 44;
    if (argc != 3) return 2;
    action = argv[2];
    if (!strcmp(argv[1], "global")) { host_mte_global_probe(); return 0; }
    if (!strcmp(argv[1], "stack")) { host_mte_stack_probe(); return 0; }
    if (!strcmp(argv[1], "heap")) { heap_probe(); return 0; }
    if (!strcmp(argv[1], "lifecycle")) return lifecycle();
    thread_heap = !strcmp(argv[1], "thread-heap");
    int explicit_stack = !strcmp(argv[1], "explicit-thread");
    if (!thread_heap && !explicit_stack && strcmp(argv[1], "thread-stack")) return 3;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    void* stack = MAP_FAILED;
    size_t size = 1024 * 1024;
    if (explicit_stack) {
        /* Bounded demonstration: does not retrofit the process loader. */
        if (prctl(55, 1UL | 2UL | (0xfffeUL << 3), 0, 0, 0)) return 45;
        stack = mmap(0, size, PROT_READ|PROT_WRITE|PROT_MTE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
        if (stack == MAP_FAILED) return 46;
        if (pthread_attr_setstack(&attr, stack, size)) return 47;
    }
    pthread_t thread;
    if (pthread_create(&thread, &attr, worker, 0)) return 48;
    int result = pthread_join(thread, 0);
    pthread_attr_destroy(&attr);
    if (stack != MAP_FAILED) munmap(stack, size);
    if (result) return 49;
    puts("THREAD_JOIN_OK");
    return 0;
}
