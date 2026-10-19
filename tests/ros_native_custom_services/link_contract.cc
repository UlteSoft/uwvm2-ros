/* SPDX-License-Identifier: Apache-2.0 */
/** This real link/invalid-argument source is NOT a Native positive witness.
 * Volatile retains the actual dispatcher address under optimizing/LTO builds.
 * A future omitted-shard link MUST fail; never replace a missing TU with a stub.
 */
#include "custom_owned_services.h"
#include "native_rcl_service_registry.h"
#include "gateways/common/native_io.h"
int main(){namespace c=uwvm2::ros_gateway::custom_owned_services;
 auto volatile factory=&c::create_selected;auto volatile bounds=&c::canonical_bounds;auto volatile validator=&c::validate_body;
 (void)factory;(void)bounds;(void)validator;return uwvm2::ros_gateway::native_io::write_all(STDOUT_FILENO,"LINKED_CUSTOM_SERVICE_DISPATCH_ONLY_NOT_NATIVE\n")?0:5;
}
