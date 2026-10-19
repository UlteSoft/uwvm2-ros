/** Genuine SDK/generated object value contracts. Authored, never run here. */
#ifdef NDEBUG
#error "Native value checks must remain active in every actual build"
#endif
#include "conversion.hpp"
#include <limits>
#include <cassert>
int main(){
 using namespace uwvm_limx;
 native::JointCmdLimx cmd{};cmd.header.seq=0xffffffffu;cmd.header.stamp.sec=0xffffffffu;cmd.header.stamp.nsec=999999999u;cmd.header.frame_id="base-\xc2\xb5";
 cmd.na=6;cmd.names={"abad_L_Joint","hip_L_Joint","knee_L_Joint","abad_R_Joint","hip_R_Joint","knee_R_Joint"};
 for(unsigned i{};i<6;++i){cmd.q.push_back(i*0.125f);cmd.v.push_back(-static_cast<float>(i)*0.25f);cmd.tau.push_back(i*0.5f);cmd.kp.push_back(i*0.75f);cmd.kd.push_back(i*0.0625f);cmd.mode.push_back(i);cmd.parallel_solver_mode.push_back(i==5?255u:i);}
 bridge::JointCmdLimx view{};assert(copy(cmd,view)==0);native::JointCmdLimx back{};assert(copy(view,back)==0);
 assert(back.header.seq==cmd.header.seq&&back.header.stamp.sec==cmd.header.stamp.sec&&back.header.stamp.nsec==cmd.header.stamp.nsec&&back.header.frame_id==cmd.header.frame_id);
 assert(back.na==cmd.na&&back.names==cmd.names&&back.q==cmd.q&&back.v==cmd.v&&back.tau==cmd.tau&&back.kp==cmd.kp&&back.kd==cmd.kd&&back.mode==cmd.mode&&back.parallel_solver_mode==cmd.parallel_solver_mode);
 auto saved=view;cmd.kd.back()=std::numeric_limits<float>::quiet_NaN();assert(copy(cmd,view)==22&&view==saved);
 cmd.kd.back()=0;cmd.parallel_solver_mode.pop_back();assert(copy(cmd,view)==22&&view==saved);
 cmd.parallel_solver_mode.push_back(1);cmd.header.stamp.nsec=1000000000;assert(copy(cmd,view)==22&&view==saved);
 cmd.header.stamp.nsec=0;cmd.header.frame_id=std::string("x\0y",3);assert(copy(cmd,view)==22&&view==saved);
 native::IMUData imu{};imu.header.seq=19;imu.header.frame_id="imu";imu.imustamp=0xffffffffffffffffull;imu.status=0xfedcba98;
 for(unsigned i{};i<3;++i){imu.euler[i]=i+0.25;imu.acc[i]=-static_cast<double>(i)-0.5;imu.gyro[i]=i+0.75;}for(unsigned i{};i<4;++i)imu.quat[i]=i*0.125;
 bridge::ImuData iv{};assert(copy(imu,iv)==0);native::IMUData ib{};assert(copy(iv,ib)==0);assert(ib.imustamp==imu.imustamp&&ib.status==imu.status&&ib.euler==imu.euler&&ib.quat==imu.quat&&ib.acc==imu.acc&&ib.gyro==imu.gyro);
 auto unchanged=iv;imu.gyro[2]=std::numeric_limits<double>::infinity();assert(copy(imu,iv)==22&&iv==unchanged);
 native::JointState state{};state.header.frame_id="base";state.na=6;state.names=back.names;state.q=back.q;state.v=back.v;state.vd=back.kd;state.tau=back.tau;
 bridge::JointState sv{};assert(copy(state,sv)==0);native::JointState sb{};assert(copy(sv,sb)==0);assert(sb.names==state.names&&sb.q==state.q&&sb.v==state.v&&sb.vd==state.vd&&sb.tau==state.tau&&sb.na==state.na);
 auto old=sv;state.tau.pop_back();assert(copy(state,sv)==22&&sv==old);
 // Nameless data remains empty, rather than fabricated names.
 state.tau.push_back(1);state.names.clear();assert(copy(state,sv)==0&&sv.names.empty());
 auto bounded_saved=sv;state.na=65;assert(copy(state,sv)==90&&sv==bounded_saved);state.na=6;
 auto prior=sv;state.header.frame_id.assign(129,'x');assert(copy(state,sv)==90&&sv==prior);
 state.header.frame_id="base";state.names={std::string(65,'x')};assert(copy(state,sv)==90&&sv==prior);
 assert(!text("\xc0\x80",64)&&!text("\xed\xa0\x80",64)&&!text("\xf4\x90\x80\x80",64));
 return 0;
}
