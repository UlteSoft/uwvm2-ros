// Minimal published preload ABI fixture. It never receives guest memory.
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#ifndef DRIVER_VALUE
#define DRIVER_VALUE 42
#endif
#ifndef DRIVER_MARKER
#define DRIVER_MARKER 'A'
#endif

static void const* wasi_api;
__attribute__((constructor)) static void constructed(void)
{
    char const* path = getenv("UWVM_DRIVER_MARKER");
    if(!path) return;
    int fd = open(path, O_WRONLY | O_CREAT | O_APPEND | O_CLOEXEC, 0600);
    if(fd >= 0) { char const mark = DRIVER_MARKER; (void)write(fd, &mark, 1); close(fd); }
}
int trusted_driver_value(void) { return DRIVER_VALUE; }
void uwvm_set_wasip1_host_api_v1(void const* value) { wasi_api = value; }

struct module_name { char const* name; size_t size; };
struct function {
    char const* name; size_t name_size;
    uint8_t const* parameters; size_t parameter_count;
    uint8_t const* results; size_t result_count;
    void (*invoke)(unsigned char*, unsigned char*);
};
struct functions { struct function const* begin; size_t size; };
static void value_call(unsigned char* result, unsigned char* ignored)
{ (void)ignored; uint32_t value = DRIVER_VALUE; memcpy(result, &value, 4); }
static void host_api_call(unsigned char* result, unsigned char* ignored)
{ (void)ignored; uint32_t value = wasi_api != NULL; memcpy(result, &value, 4); }
struct module_name uwvm_get_module_name(void)
{ return (struct module_name){"driver", 6}; }
struct functions uwvm_function(void)
{
    static uint8_t const i32_result[] = {0x7f};
    static struct function const functions[] = {
        {"value", 5, NULL, 0, i32_result, 1, value_call},
        {"host_api_present", 16, NULL, 0, i32_result, 1, host_api_call}
    };
    return (struct functions){functions, 2};
}
