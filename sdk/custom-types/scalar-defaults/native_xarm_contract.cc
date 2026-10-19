/* SPDX-License-Identifier: Apache-2.0 */
/** Actual generated xArm scalar-default contract, authored and not executed.
 * Build only against the selected original SDK and a complete freshly bound
 * catalog. This component creates no ROS endpoint and grants no robot command.
 * Native defaults initialize fresh values; complete decode replaces every field
 * and never fills a missing/truncated wire payload from those defaults.
 */
#include <xarm_msgs/srv/get_set_modbus_data.hpp>
#include <native_ros2.hpp>
#include <bounded_records.h>
#include <cstdlib>
#include <vector>
namespace {
using Native=xarm_msgs::srv::GetSetModbusData;
using Request=Native::Request;using Response=Native::Response;
using RequestCodec=uwvm_custom_bounded_native::Ros2XarmMsgsSrvGetSetModbusData_RequestCodec;
using ResponseCodec=uwvm_custom_bounded_native::Ros2XarmMsgsSrvGetSetModbusData_ResponseCodec;
using Record=roswasmc_bounded_2_xarm_msgs_srv_GetSetModbusData_Request;
void need(bool value){if(!value)std::abort();}
}
int main(){
 Request original;need(original.modbus_length==0&&original.host_id==9&&
  !original.is_transparent_transmission&&!original.use_503_port);
 Record selected{};need(roswasmc_bounded_2_xarm_msgs_srv_GetSetModbusData_Request_default(&selected)==0);
 need(selected.modbus_length==original.modbus_length&&selected.host_id==original.host_id&&
  selected.is_transparent_transmission==original.is_transparent_transmission&&selected.use_503_port==original.use_503_port);
 Record converted{};need(RequestCodec::to_record(original,converted));
 std::vector<unsigned char> first;need(RequestCodec::encode(original,first));
 std::vector<unsigned char> second(first.size());std::size_t used{};
 need(!roswasmc_bounded_2_xarm_msgs_srv_GetSetModbusData_Request_encode(&selected,second.data(),second.size(),&used)&&used==second.size()&&first==second);
 // Every nondefault field remains a wire value; declared defaults are not
 // equality restrictions and are never applied after complete decoding.
 Request other;other.modbus_data={1,0,255};other.modbus_length=-7;other.ret_length=27;other.host_id=10;
 other.is_transparent_transmission=true;other.use_503_port=true;
 std::vector<unsigned char> full;need(RequestCodec::encode(other,full));
 need(RequestCodec::decode(full.data(),full.size(),original));
 need(original.modbus_data==other.modbus_data&&original.modbus_length==-7&&original.ret_length==27&&
  original.host_id==10&&original.is_transparent_transmission&&original.use_503_port);
 auto before=original;need(!full.empty());need(!RequestCodec::decode(full.data(),full.size()-1,original));
 need(original==before);full.push_back(0);need(!RequestCodec::decode(full.data(),full.size(),original)&&original==before);
 Response response;response.ret=-2;response.message="exact native response";response.ret_data={0,255,4};
 std::vector<unsigned char> wire;need(ResponseCodec::encode(response,wire));Response copy;
 need(ResponseCodec::decode(wire.data(),wire.size(),copy)&&copy==response);
 return 0;
}
