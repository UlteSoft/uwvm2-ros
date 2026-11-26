#include <pthread.h>
#include <sched.h>
#include <stdatomic.h>
#include <stdint.h>
#include <stdio.h>

static _Thread_local volatile uint32_t tls_value = 11;
static pthread_key_t key;
static atomic_uint ready, proceed;

struct instance {
    unsigned id;
    uintptr_t tls_address;
    uintptr_t stack_address;
    pthread_t self;
};

static void* run(void* argument)
{
    struct instance* state = argument;
    volatile uint32_t stack[32];
    unsigned id = state->id;
    int fresh = tls_value == 11 && pthread_getspecific(key) == NULL;
    int stored = pthread_setspecific(key, argument) == 0;
    state->self = pthread_self();
    state->tls_address = (uintptr_t)&tls_value;
    state->stack_address = (uintptr_t)&stack[0];
    tls_value = 200 + id;
    stack[17] = 100 + id;
    atomic_fetch_add_explicit(&ready, 1, memory_order_release);
    while (!atomic_load_explicit(&proceed, memory_order_acquire))
        sched_yield();
    return fresh && stored && tls_value == 200 + id && stack[17] == 100 + id &&
           pthread_getspecific(key) == argument ? argument : NULL;
}

int main(void)
{
    struct instance states[3] = {{.id = 1}, {.id = 2}, {.id = 3}};
    pthread_t workers[3];
    pthread_attr_t attributes;
    void* result = NULL;
    tls_value = 99;
    if (pthread_key_create(&key, NULL) || pthread_setspecific(key, &attributes) ||
        pthread_attr_init(&attributes) || pthread_attr_setstacksize(&attributes, 256 * 1024))
        return 1;
    for (unsigned i = 0; i < 2; ++i)
        if (pthread_create(&workers[i], &attributes, run, &states[i])) return 2;
    while (atomic_load_explicit(&ready, memory_order_acquire) != 2)
        sched_yield();
    if (states[0].tls_address == states[1].tls_address || states[0].stack_address == states[1].stack_address ||
        states[0].tls_address == (uintptr_t)&tls_value || states[1].tls_address == (uintptr_t)&tls_value ||
        pthread_equal(states[0].self, states[1].self) || pthread_equal(states[0].self, pthread_self()) ||
        pthread_equal(states[1].self, pthread_self()))
        return 3;
    atomic_store_explicit(&proceed, 1, memory_order_release);
    for (unsigned i = 0; i < 2; ++i)
        if (pthread_join(workers[i], &result) || result != &states[i]) return 4;
    if (pthread_create(&workers[2], &attributes, run, &states[2]) ||
        pthread_join(workers[2], &result) || result != &states[2])
        return 5;
    if (tls_value != 99 || pthread_getspecific(key) != &attributes ||
        pthread_attr_destroy(&attributes) || pthread_key_delete(key))
        return 6;
    puts("pthread TLS, stack and joins passed");
    return 0;
}
