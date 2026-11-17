/** @file @brief Test peer transport for a deployment-owned Python service process. */
#include <uwvm2/imported/ros/scene/attachment_tls.h>
#include <memory>
namespace s=uwvm2::imported::ros::scene;
using endpoint=s::attachment_tls::channel;
extern "C" void* wros_test_accept(int fd,int key,std::uint32_t pid,unsigned purpose,
                                    std::uint8_t const* binding,std::size_t frame) noexcept
{
    try {
        if(!binding||purpose<1||purpose>2)return nullptr;
        s::wire::reader fields{{binding,112}};
        std::array<s::attachment_tls::id,5> ids{};
        s::attachment_tls::digest contract{};
        for(auto& pair:ids)for(auto& word:pair)if(!fields.u64(word))return nullptr;
        for(auto& word:contract)if(!fields.u64(word))return nullptr;
        auto peer=uwvm2::utils::container::make_unique<endpoint>();
        auto result=peer->bind(fd,{pid,::getuid(),::getgid()},key,static_cast<s::attachment_tls::purpose>(purpose),
                               ids[0],ids[1],ids[2],ids[3],ids[4],contract,frame,true);
        if(result!=s::attachment_tls::error::success)return nullptr;
        return peer.release();
    }catch(...){return nullptr;}
}
extern "C" void wros_test_close(void* channel) noexcept
{uwvm2::utils::container::std_unique_ptr<endpoint> owner{static_cast<endpoint*>(channel)};}
extern "C" int wros_test_send(void* channel,std::uint8_t const* data,std::size_t size) noexcept
{
    if(!channel||!data||size<176)return 22;
    auto packet=std::span{data,size};
    return static_cast<int>(static_cast<endpoint*>(channel)->send(packet.first(176),packet.subspan(176)));
}
extern "C" int wros_test_receive(void* channel,std::uint8_t* data,std::size_t capacity,std::size_t* count) noexcept
{
    if(!channel||!data||!count)return 22;
    return static_cast<int>(static_cast<endpoint*>(channel)->receive({data,capacity},*count));
}
