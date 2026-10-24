/* Positive, finite Wasm pthread consumer; no ROS imports or native robot operation. */
#include "platform/ros/c/roswasm/library_profile.h"
#include <pthread.h>
#include <stdio.h>

struct thread_case
{
    pthread_t parent;
    int value;
    unsigned different_thread;
};

static void* increment(void* opaque)
{
    struct thread_case* state = (struct thread_case*)opaque;
    state->different_thread = !pthread_equal(state->parent, pthread_self());
    state->value += 5;
    return opaque;
}

int main(void)
{
    struct thread_case state;
    pthread_t child;
    void* result = 0;
    if (roswasmc_library_profile() != ROSWASM_C_PROFILE)
        return 1;
    state.parent = pthread_self();
    state.value = 37;
    state.different_thread = 0;
    if (pthread_create(&child, 0, increment, &state) != 0)
        return 2;
    if (pthread_join(child, &result) != 0)
        return 3;
    if (result != &state || state.value != 42 || state.different_thread != 1)
        return 4;
    return printf("c-pthread=42\n") < 0 ? 5 : 0;
}
