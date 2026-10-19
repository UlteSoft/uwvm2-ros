/** Complete local response view/copy checks and actual Guest default denial.
 *
 * Include the actual Yaskawa C reactor without adding a production test import.
 * Local fixtures exercise every response getter, signed/unsigned values and
 * full-capacity diagnostics. They are not Service delivery or controller data.
 * The final initialization runs the real zero-grant path and never submits IO.
 */
#ifndef UWVM_INDUSTRIAL_C_SOURCE
#error "Select the exact actual Yaskawa query example source"
#endif
#include UWVM_INDUSTRIAL_C_SOURCE
#include <limits.h>

int main(void)
{
    static uint32_t const lengths[] = {0, 1, 32, 511, 512};
    unsigned char destination[514], untouched[514];
    unsigned i, j;
    _Static_assert(sizeof response.message.data == 512, "select the exact reviewed Yaskawa message capacity");
    if(response_ready() || message_size() || copy_message(0, 0) != 9) return 1;
    memset(untouched, 0xA5, sizeof untouched);
    for(i = 0; i != sizeof lengths / sizeof lengths[0]; ++i)
    {
        uint32_t length = lengths[i];
        response.result_code = UINT32_MAX - i;
        response.success = (i & 1) != 0;
        response.value = INT32_MIN + (int32_t)i * 12345;
        response.message.size = length;
        for(j = 0; j != length; ++j) response.message.data[j] = (unsigned char)(32 + j % 90);
        ready = 1;
        if(!response_ready() || result_code() != UINT32_MAX - i ||
           response_success() != (i & 1) || io_value() != INT32_MIN + (int32_t)i * 12345 ||
           message_size() != length) return 2;
        memcpy(destination, untouched, sizeof destination);
        if(length && (copy_message(destination, length - 1) != 90 ||
                      memcmp(destination, untouched, sizeof destination))) return 3;
        if(copy_message(destination, length)) return 4;
        if(memcmp(destination, response.message.data, length) ||
           memcmp(destination + length, untouched + length, sizeof destination - length)) return 5;
        if(copy_message(0, length) != (length ? 22 : 0)) return 6;
    }
    response.message.size = 513;
    memcpy(destination, untouched, sizeof destination);
    if(copy_message(destination, sizeof destination) != 22 ||
       memcmp(destination, untouched, sizeof destination)) return 10;
    response = (roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Response){0};
    ready = 0;
    if(initialize_case(2) != 22 || initialize_case(0) != 0) return 7;
    if(denied_verified() != 1 || primary_status() || cleanup_status()) return 8;
    if(response_ready() || query_once() != 9 || close_case() != 9 || initialize_case(0) != 9) return 9;
    return 0;
}
