/** @file
 * @brief Pure test-owned import model for lifetime/failure contracts.
 * No native ROS, DDS/TCPROS, Guest or Wasm behavior follows from these tests.
 * Each open returns a distinct ID; close retires it even if cleanup fails.
 */
#ifndef ROSWASM_OWNERSHIP_MODEL_BACKEND_H
#define ROSWASM_OWNERSHIP_MODEL_BACKEND_H
#include <platform/ros/c/roswasm/node.h>
#include <string.h>
static unsigned fixture_opens,fixture_closes,fixture_activations,fixture_requests,fixture_polls,fixture_publishes;
static uint64_t fixture_next=1;
static unsigned char fixture_live[128],fixture_payload[256];
static uint32_t fixture_close_errors[128],fixture_activate_error,fixture_request_error;
static size_t fixture_payload_size;
static char const* fixture_denied="denied";
/** @brief Store an unaligned complete eight-byte little-endian model slot. */
static void fixture_store64(uint64_t output,uint64_t value) {
    unsigned char* bytes=(unsigned char*)(uintptr_t)output;unsigned n;
    for(n=0;n<8;++n) bytes[n]=(unsigned char)(value>>(8*n));
}
/** @brief ABI version value is1, independent of ROS backend selection. */
static uint32_t fixture_version(void) {return 1;}
/** @brief Explicit model aliases only; denial preserves the output slot. */
static uint32_t fixture_open(uint32_t memory,uint64_t alias,uint64_t length,uint64_t output) {
    (void)memory;++fixture_opens;
    if(length==strlen(fixture_denied)&&!memcmp((void const*)(uintptr_t)alias,fixture_denied,(size_t)length)) return 13;
    if(!length||fixture_next>=128) return 22;
    fixture_live[fixture_next]=1;fixture_store64(output,fixture_next++);return 0;
}
/** @brief Retire before reporting a configured notification failure. */
static uint32_t fixture_close(uint64_t handle) {
    ++fixture_closes;if(!handle||handle>=128||!fixture_live[handle]) return 9;
    fixture_live[handle]=0;return fixture_close_errors[handle];
}
/** @brief Revocation uses the same retirement model, without recovery. */
static uint32_t fixture_revoke(uint64_t handle) {return fixture_close(handle);}
/** @brief Independently count activation and failure after acquisition. */
static uint32_t fixture_activate(uint64_t handle) {
    if(!handle||handle>=128||!fixture_live[handle]) return 9;
    ++fixture_activations;return fixture_activate_error;
}
/** @brief Count one pure canonical write; no native endpoint exists. */
static uint32_t fixture_publish(uint64_t handle,uint32_t memory,uint64_t bytes,uint64_t length) {
    (void)memory;(void)bytes;(void)length;
    if(!handle||handle>=128||!fixture_live[handle]) return 9;
    ++fixture_publishes;return fixture_request_error;
}
/** @brief Copy one event or return an error preserving every output byte. */
static uint32_t fixture_poll(uint64_t handle,uint32_t memory,uint64_t output,uint64_t capacity,uint64_t slot) {
    (void)memory;if(!handle||handle>=128||!fixture_live[handle]) return 9;
    ++fixture_polls;if(fixture_request_error) return fixture_request_error;
    fixture_store64(slot,fixture_payload_size);if(fixture_payload_size>capacity) return 90;
    if(fixture_payload_size) memcpy((void*)(uintptr_t)output,fixture_payload,fixture_payload_size);return 0;
}
/** @brief One RPC dispatch, without timeout/transport replay. */
static uint32_t fixture_request(uint64_t h,uint32_t memory,uint64_t input,uint64_t input_size,
    uint64_t output,uint64_t capacity,uint64_t slot) {
    (void)input;(void)input_size;++fixture_requests;return fixture_poll(h,memory,output,capacity,slot);
}
/** @brief Action advertisement independently counts request and activation. */
static uint32_t fixture_action_advertise(uint64_t h,uint32_t memory,uint64_t input,uint64_t input_size,
    uint64_t output,uint64_t capacity,uint64_t slot) {
    uint32_t code;(void)memory;(void)input;(void)input_size;(void)output;(void)capacity;
    ++fixture_requests;code=fixture_activate(h);if(!code) fixture_store64(slot,0);return code;
}
/** @brief Complete test table containing no real imported symbols. */
static roswasmc_backend* fixture_backend(void) {
    static roswasmc_backend backend;unsigned n;
    backend.version=2;backend.abi_version=fixture_version;backend.open=fixture_open;
    backend.close=fixture_close;backend.revoke=fixture_revoke;backend.publish=fixture_publish;
    backend.subscribe=fixture_activate;backend.poll=fixture_poll;backend.advertise_service=fixture_activate;
    backend.subscribe_parameter_events=fixture_activate;backend.log=fixture_publish;
    for(n=0;n<ROSWASMC_OPERATION_COUNT;++n) backend.request[n]=fixture_request;
    backend.request[ROSWASMC_ACTION_ADVERTISE]=fixture_action_advertise;return &backend;
}
#endif
