/** Independent before/after regression reference: the whole old decoding
 * function below is byte-identical to the admitted original except its name.
 * Original image.h SHA256: 524c5c483935cecd9a14c0042ea0c7c56450c78d1367201b170a864053ef4b8d.
 * It is a test oracle, never another production Image validator/codec.
 * Authored only; no compile/run/native compatibility credit is implied.
 */
#include <uwvm2/imported/ros/ipc/image.h>
#include <array>
#include <cassert>
#ifdef NDEBUG
#error "Image policy contracts require active assertions"
#endif
namespace ipc=uwvm2::imported::ros::ipc;
namespace reference {
using ipc::image_view;using ipc::get;
    inline bool legacy_decode_image_view(std::uint8_t const* bytes,std::size_t size,std::uint8_t backend,image_view& out) noexcept
    {
        if(!bytes||!size||backend<1||backend>2) {return false;}
        image_view value;
        std::size_t p{};
        auto number=[&](std::uint32_t& n) { if(p>size||size-p<4) { return false; } n=static_cast<std::uint32_t>(get(bytes+p,4)); p+=4; return true; };
        auto string=[&](std::string_view& s) { std::uint32_t n{}; if(!number(n)||n>255||p>size||n>size-p) { return false; } s={reinterpret_cast<char const*>(bytes+p),n}; p+=n; return true; };
        if(!number(value.sequence)||!number(value.seconds)||!number(value.nanoseconds)||!string(value.frame_id)||!number(value.height)||!number(value.width)||!string(value.encoding)||p==size) { return false; }
        if(value.nanoseconds>=1000000000||(backend==2&&value.seconds>2147483647)||!value.height||!value.width||value.frame_id.empty()) { return false; }
        for(char c:value.frame_id) { if(!((c>='a'&&c<='z')||(c>='A'&&c<='Z')||(c>='0'&&c<='9')||c=='_'||c=='/')) { return false; } }
        if(value.frame_id.front()=='/'||value.frame_id.back()=='/'||value.frame_id.find("//")!=std::string_view::npos) { return false; }
        auto e{bytes[p++]}; if(e>1) { return false; } value.bigendian=e!=0;
        std::uint32_t n{};
        if(!number(value.step)||!number(n)||p>size||n!=size-p) { return false; }
        std::uint64_t stride{};
        if(value.encoding=="mono8") { stride=1; }
        else if(value.encoding=="mono16") { stride=2; }
        else if(value.encoding=="rgb8"||value.encoding=="bgr8") { stride=3; }
        else if(value.encoding=="rgba8"||value.encoding=="bgra8") { stride=4; }
        else { return false; }
        if(std::uint64_t{value.width}*stride>value.step||std::uint64_t{value.step}*value.height!=n) { return false; }
        value.data=bytes+p;value.data_size=n;out=value;return true;
    }
}
static void same_decode(std::vector<std::uint8_t>const& bytes,unsigned backend){
    ipc::image_view old{},next{};old.width=111;next.width=222;
    bool a=reference::legacy_decode_image_view(bytes.data(),bytes.size(),static_cast<std::uint8_t>(backend),old);
    bool b=ipc::decode_image_view(bytes.data(),bytes.size(),static_cast<std::uint8_t>(backend),next);
    assert(a==b);
    if(a){assert(ipc::valid_image_view(next,static_cast<std::uint8_t>(backend)));
      assert(old.sequence==next.sequence&&old.seconds==next.seconds&&old.nanoseconds==next.nanoseconds&&old.height==next.height&&old.width==next.width&&old.step==next.step&&old.frame_id==next.frame_id&&old.encoding==next.encoding&&old.bigendian==next.bigendian&&old.data==next.data&&old.data_size==next.data_size);}
    else{assert(old.width==111&&next.width==222);}
}
int main(){
 std::array<std::uint8_t,64>pixels{};
 for(auto encoding:{"mono8","mono16","rgb8","bgr8","rgba8","bgra8","32FC1","", "unknown"})
 for(auto frame:{"camera","camera/optical","/camera","camera/","camera//optical","", "a-b"})
 for(unsigned width:{0u,1u,2u})for(unsigned height:{0u,1u,2u})for(unsigned step:{0u,1u,2u,3u,4u,8u}){
   auto n=static_cast<std::size_t>(step*height);ipc::image_view v{7,42,17,height,width,step,frame,encoding,false,pixels.data(),n};
   auto bytes=ipc::encode_image_view(v);for(unsigned backend:{0u,1u,2u,3u})same_decode(bytes,backend);
 }
 ipc::image_view valid{0,42,17,1,1,1,"camera","mono8",false,pixels.data(),1};auto bytes=ipc::encode_image_view(valid);
 assert(ipc::valid_image_view(valid,2));
 for(std::size_t n{};n<bytes.size();++n){std::vector<std::uint8_t>shorter(bytes.begin(),bytes.begin()+n);same_decode(shorter,2);}
 auto trailing=bytes;trailing.push_back(0);same_decode(trailing,2);
 valid.seconds=UINT32_MAX;auto signed_seconds=ipc::encode_image_view(valid);same_decode(signed_seconds,1);same_decode(signed_seconds,2);assert(!ipc::valid_image_view(valid,2));
 valid.seconds=42;valid.nanoseconds=1000000000;auto bad_ns=ipc::encode_image_view(valid);same_decode(bad_ns,2);assert(!ipc::valid_image_view(valid,2));
 valid.nanoseconds=17;valid.data=nullptr;assert(!ipc::valid_image_view(valid,2));valid.data=pixels.data();valid.data_size=2;assert(!ipc::valid_image_view(valid,2));
}
