/** @file
 * @brief Only basic stdout for the ROS command/reactor flavor.
 * Pure computation libraries do not compile or import this file.
 */
#include "engine.h"
struct vector { uint32_t address, length; };
_Static_assert(sizeof(struct vector)==8,"WASIp1 iovec");
__attribute__((import_module("wasi_snapshot_preview1"),import_name("fd_write")))
extern uint32_t fd_write(uint32_t,struct vector const *,uint32_t,uint32_t *);
__attribute__((import_module("wasi_snapshot_preview1"),import_name("fd_read")))
extern uint32_t fd_read(uint32_t,struct vector const *,uint32_t,uint32_t *);
uint32_t uwvm_js_basic_write(uint8_t const *bytes,size_t size)
{
    if (size>UWVM_JS_PRINT_BYTES) return 28;
    uint32_t offset=0, interrupts=0;
    while (offset<size) {
        struct vector part={(uint32_t)(uintptr_t)(bytes+offset),(uint32_t)(size-offset)};
        uint32_t actual=0, status=fd_write(1,&part,1,&actual);
        if (status==27 && ++interrupts<=16) continue;
        if (status || !actual || actual>size-offset) return status ? status : 29;
        offset+=actual; interrupts=0;
    }
    return 0;
}
/** One explicit supervisor gate byte. EOF, EAGAIN and failures are terminal;
 * no unbounded buffer, host timer or hidden request replay is introduced.
 */
uint32_t uwvm_js_basic_read(uint8_t *byte)
{
    struct vector part={(uint32_t)(uintptr_t)byte,1};
    for (unsigned attempt=0;attempt<17;++attempt) {
        uint32_t actual=0, status=fd_read(0,&part,1,&actual);
        if (status==27) continue;
        return status ? status : actual==1 ? 0 : 29;
    }
    return 27;
}
