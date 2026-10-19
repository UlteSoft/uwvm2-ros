/* SPDX-License-Identifier: Apache-2.0
 * Explicit real optional-table selection. No policy, endpoint or native query
 * is created just by including a header or obtaining the static dispatch table.
 */
#include <roswasm/ros2.h>
#include <roswasm/service_client_ros2.h>
#include <roswasm/service_client.hpp>
int main(){
  roswasmc_context context{roswasmc_ros2_backend()};
  if(!context.backend->abi_version||context.backend->abi_version()!=1)return 95;
  auto const& api=*roswasmc_ros2_service_client_backend();if(api.abi_version()!=1)return 95;
  roswasmcpp::Capability source{context,"service-call"};if(source.error())return static_cast<int>(source.error());
  roswasmcpp::ServiceAvailability query{context,api,source,"service-available"};
  std::uint32_t code=query.error();std::uint64_t bits{};bool ready{};
  if(!code)code=query.features(bits);
  if(!code&&(bits&UWVM_ROS_CLIENT_TYPED_QUERY)==0)code=95;
  if(!code)code=query.query(UINT64_C(1000000000),ready);
  auto query_close=query.close();auto source_close=source.close();
  if(code)return static_cast<int>(code);
  if(query_close||source_close)return static_cast<int>(query_close?query_close:source_close);
  return ready?0:1;
}
