/* SPDX-License-Identifier: Apache-2.0
 * Value-only original-rule cases; actual imports are never mocked. */
#include "lite3_services.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
int main(void) {
    static int32_t const rates[] = {
        0, 1, 2, 4, 5, 8, 10, 20, 25, 40, 50, 100, 125, 200};
    static int32_t const invalid[] = {
        -1, 3, 7, 199, 201, 250, 1000, INT32_MIN, INT32_MAX};
    uwvm_deep_lite3_request value = {0}, decoded = {0};
    unsigned char bytes[4];
    size_t used = 0, i;
    for (i = 0; i < sizeof rates / sizeof *rates; ++i) {
        assert(uwvm_deep_lite3_prepare_sdk_mode(&value, rates[i]) == 0 &&
               value.command == rates[i]);
        assert(roswasmc_bounded_2_drdds_srv_StdSrvInt32_Request_encode(
                   &value, bytes, sizeof bytes, &used) == 0 &&
               used == 4);
        assert(uwvm_bounded_get(bytes, 4) == (uint32_t)rates[i]);
        assert(roswasmc_bounded_2_drdds_srv_StdSrvInt32_Request_decode(
                   bytes, used, &decoded) == 0 &&
               decoded.command == rates[i]);
    }
    for (i = 0; i < sizeof invalid / sizeof *invalid; ++i) {
        value.command = -17;
        assert(uwvm_deep_lite3_prepare_sdk_mode(&value, invalid[i]) == 22 &&
               value.command == -17);
    }
    assert(uwvm_deep_lite3_prepare_emergency_stop(&value) == 0 && value.command == 0);
    {
        uwvm_deep_lite3_response response = {INT32_MIN}, next = {1};
        assert(roswasmc_bounded_2_drdds_srv_StdSrvInt32_Response_encode(
                   &response, bytes, sizeof bytes, &used) == 0);
        assert(roswasmc_bounded_2_drdds_srv_StdSrvInt32_Response_decode(
                   bytes, used, &next) == 0 &&
               next.result == INT32_MIN);
    }
    puts("LITE3_SERVICE_VALUES_OK rates=14 invalid=9 native_result_bits=32");
    return 0;
}
