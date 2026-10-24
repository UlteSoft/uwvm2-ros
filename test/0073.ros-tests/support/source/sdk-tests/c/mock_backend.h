/** @file
 * @brief Test-only pure import model. This opens no ROS runtime or native resource.
 * Compile/run these tests in the authorized Linux cgroup before reporting actual
 * build success. A model pass establishes wrapper/codec behavior only, not DDS,
 * ROS1 networking, Action native lifetime, ABI interpreter or ARM execution.
 */
#ifndef ROSWASM_TEST_MOCK_BACKEND_H
#define ROSWASM_TEST_MOCK_BACKEND_H
#include <platform/ros/c/roswasm/roswasmc.h>
#include <string.h>
/** @brief Explicit model state: error90 may write a size or may leave the slot untouched. */
static uint32_t model_error;
static int model_write_length;
/** @brief Deliberate malformed-success fixture: 1=unwritten length, 2=length beyond capacity. */
static int model_malformed_success;
static unsigned model_close_calls,model_publish_calls,model_callback_calls;
static unsigned char model_payload[256];
static size_t model_payload_size;
/** @brief Store one test-owned output slot using the actual 8LE contract. */
static void model_store64(uint64_t offset,uint64_t value) {
    unsigned i;unsigned char* bytes=(unsigned char*)(uintptr_t)offset;
    for(i=0;i<8;++i) bytes[i]=(unsigned char)(value>>(8*i));
}
/** @brief ABI version returns the value1. */
static uint32_t model_version(void) {return 1;}
/** @brief Only alias `allowed` exists in this fixture, so all others are denied. */
static uint32_t model_open(uint32_t memory,uint64_t alias,uint64_t length,uint64_t output) {
    (void)memory;if(length!=7||memcmp((void const*)(uintptr_t)alias,"allowed",7)) return UWVM_ROS_DENIED;
    model_store64(output,UINT64_C(0x0102030405060708));return 0;
}
/** @brief Model a handle retired before transport cleanup failure. */
static uint32_t model_close(uint64_t handle) {(void)handle;++model_close_calls;return model_error;}
/** @brief Explicit revocation uses the same local-retirement model. */
static uint32_t model_revoke(uint64_t handle) {(void)handle;return model_error;}
/** @brief Count publication attempts without creating any endpoint. */
static uint32_t model_publish(uint64_t h,uint32_t m,uint64_t offset,uint64_t size) {
    (void)h;(void)m;(void)offset;(void)size;++model_publish_calls;return model_error;
}
/** @brief Queue activation is a model operation without host callbacks. */
static uint32_t model_activate(uint64_t handle) {(void)handle;return model_error;}
/** @brief Copy one model event or return an unwritten output-slot error. */
static uint32_t model_poll(uint64_t h,uint32_t m,uint64_t output,uint64_t capacity,uint64_t length) {
    (void)h;(void)m;
    if(model_error) {if(model_write_length) model_store64(length,model_payload_size);return model_error;}
    if(model_malformed_success==1) return 0;
    if(model_malformed_success==2) {model_store64(length,capacity+1);return 0;}
    model_store64(length,model_payload_size);if(model_payload_size>capacity) return UWVM_ROS_TOO_LARGE;
    if(model_payload_size) memcpy((void*)(uintptr_t)output,model_payload,model_payload_size);return 0;
}
/** @brief Model a generic synchronous request using the same output-slot branches. */
static uint32_t model_request(uint64_t h,uint32_t m,uint64_t input,uint64_t input_size,uint64_t output,uint64_t capacity,uint64_t length) {
    (void)input;(void)input_size;return model_poll(h,m,output,capacity,length);
}
/** @brief Return a complete test dispatch table with every request mapped to the pure model. */
static roswasmc_backend const* model_backend(void) {
    static roswasmc_backend backend;unsigned i;
    backend.version=2;backend.abi_version=model_version;backend.open=model_open;
    backend.close=model_close;backend.revoke=model_revoke;backend.publish=model_publish;
    backend.subscribe=model_activate;backend.poll=model_poll;backend.advertise_service=model_activate;
    backend.subscribe_parameter_events=model_activate;backend.log=model_publish;
    for(i=0;i<ROSWASMC_OPERATION_COUNT;++i) backend.request[i]=model_request;return &backend;
}
#endif
