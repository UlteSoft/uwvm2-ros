/** Real C-value/C++ SDK Codec path; ordinary T{} remains zero storage. */
#include "bounded_records.hpp"
#include <span>
int main(){
    using Q=uwvm_custom::Ros2Geometry_msgsMsgQuaternion;using B=uwvm_custom::Ros2Default_fixtureMsgBundle;
    Q zero{};if(zero.w!=0)return 1;
    auto q=roswasmcpp::Codec<Q>::default_value();auto b=roswasmcpp::Codec<B>::default_value();
    if(q.w!=1||b.fixed[0].w!=1||b.fixed[1].w!=1||b.pose.orientation.w!=1||b.dynamic.size)return 2;
    b.fixed[0].w=.25;if(b.fixed[1].w!=1)return 3;
    roswasmcpp::wire::bytes bytes;
    if(!roswasmcpp::Codec<B>::encode(b,bytes))return 4;
    B out{};if(!roswasmcpp::Codec<B>::decode(std::span<unsigned char const>(bytes.data(),bytes.size()),out)||out.fixed[0].w!=.25||out.fixed[1].w!=1||out.pose.orientation.w!=1)return 5;
    roswasmcpp::wire::bytes short_bytes(31);q.w=9;
    if(roswasmcpp::Codec<Q>::decode(std::span<unsigned char const>(short_bytes.data(),short_bytes.size()),q)||q.w!=9)return 6;
    roswasmcpp::wire::bytes zero_bytes(32);if(!roswasmcpp::Codec<Q>::decode(std::span<unsigned char const>(zero_bytes.data(),zero_bytes.size()),q)||q.w!=0)return 7;
    return 0;
}
