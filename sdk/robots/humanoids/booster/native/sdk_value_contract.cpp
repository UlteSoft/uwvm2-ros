/** Actual SDK unsigned fields and distinct ROS2 bridge values, no fake types. */
#ifdef NDEBUG
#error "Native value checks must remain active"
#endif
#include "sdk_conversion.hpp"
#include <limits>
#include <cassert>
int main(){
 using namespace uwvm_booster;
 NativeState source;source.motor_state_parallel().resize(23);source.motor_state_serial().resize(23);
 for(unsigned i{};i<3;++i){source.imu_state().rpy()[i]=i*0.125f;source.imu_state().gyro()[i]=-static_cast<float>(i)*0.25f;source.imu_state().acc()[i]=i+0.5f;}
 auto fill=[](auto& rows,float bias){for(unsigned i{};i<rows.size();++i){auto& m=rows[i];m.mode(255u-i);m.q(i+bias);m.dq(-static_cast<float>(i)-bias);m.ddq(i*0.25f);m.tau_est(i*0.5f);m.temperature(255u-i);m.lost(0xffffffffu-i);m.reserve()[0]=0xabcdef01u+i;m.reserve()[1]=0xfedcba98u-i;}};
 fill(source.motor_state_parallel(),0.125f);fill(source.motor_state_serial(),0.375f);
 State message{};assert(copy(source,message)==0);assert(message.motor_state_serial[0].mode==255&&message.motor_state_serial[0].temperature==255);
 NativeState restored;assert(copy(message,restored)==0);State again{};assert(copy(restored,again)==0);assert(again==message);
 auto saved=message;source.motor_state_parallel()[22].tau_est(std::numeric_limits<float>::infinity());assert(copy(source,message)==22&&message==saved);
 source.motor_state_parallel().resize(30);assert(copy(source,message)==90&&message==saved);
 NativeCommand command;command.cmd_type(::booster_interface::msg::SERIAL);command.motor_cmd().resize(23);
 for(unsigned i{};i<23;++i){auto& m=command.motor_cmd()[i];m.mode(255u-i);m.q(i*0.125f);m.dq(-static_cast<float>(i)*0.25f);m.tau(i*0.5f);m.kp(i+0.75f);m.kd(i+0.0625f);m.weight(i/32.0f);}
 Command cv{};assert(copy(command,cv)==0&&cv.cmd_type==1&&cv.motor_cmd[0].mode==255);
 NativeCommand cr;assert(copy(cv,cr)==0);Command ca{};assert(copy(cr,ca)==0&&ca==cv);
 auto original=cv;command.motor_cmd()[22].weight(std::numeric_limits<float>::quiet_NaN());assert(copy(command,cv)==22&&cv==original);
 cv=original;cv.cmd_type=2;assert(copy(cv,cr)==22);Command native_after{};assert(copy(cr,native_after)==0&&native_after==original);
 return 0;
}
