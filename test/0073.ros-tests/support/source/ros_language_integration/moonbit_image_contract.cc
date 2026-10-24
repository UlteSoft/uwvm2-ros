// Independent native Host-codec witness for the MoonBit Image fixture.
// This checks canonical interoperability; it does not start native ROS/DDS.
#include "src/uwvm2/imported/ros/ipc/image.h"
#include <cstdio>
#include <stdexcept>

int main()
{
    namespace ipc = uwvm2::imported::ros::ipc;
    unsigned checks{};
    auto require = [&](bool value) {
        ++checks;
        if(!value) { throw std::runtime_error("native Image contract mismatch"); }
    };
    try
    {
        ipc::image rgb{0,1,2,2,2,8,"camera/optical","rgb8",false,
            {255,0,0,0,255,0,170,187,0,0,255,255,255,255,204,221}};
        auto bytes{ipc::encode_image(rgb)};
        // The MoonBit test's exact vector is independently assembled in Python
        // by the Linux reader. Both encoders must produce that complete value.
        for(auto b : bytes) { std::printf("%02x", b); }
        std::putchar('\n');
        ipc::image decoded{};
        require(ipc::decode_image(bytes,1,decoded));
        require(ipc::decode_image(bytes,2,decoded));
        require(decoded.data==rgb.data && decoded.step==8 && decoded.frame_id==rgb.frame_id);
        for(std::size_t n{}; n<bytes.size(); ++n)
        {
            ipc::image_view view{};
            require(!ipc::decode_image_view(bytes.data(),n,2,view));
        }
        auto bad{bytes};bad.push_back(0);require(!ipc::decode_image(bad,2,decoded));
        bad=bytes;bad[46]=2;require(!ipc::decode_image(bad,2,decoded));
        bad=bytes;for(unsigned i=12;i<16;++i){bad[i]=255;}
        require(!ipc::decode_image(bad,2,decoded));
        rgb.seconds=UINT32_MAX;rgb.sequence=UINT32_MAX;
        require(ipc::decode_image(ipc::encode_image(rgb),1,decoded));
        require(!ipc::decode_image(ipc::encode_image(rgb),2,decoded));
        // Native Host accepts canonical sequence bits for ROS2; the MoonBit
        // public ROS2 profile deliberately requires zero to avoid data loss
        // when projecting to ROS2's Header, which has no sequence field.
        rgb.seconds=1;require(ipc::decode_image(ipc::encode_image(rgb),2,decoded));
        ipc::image mono{0,1,2,2,2,2,"camera/optical","mono8",false,{76,149,28,255}};
        require(ipc::decode_image(ipc::encode_image(mono),2,decoded));
        require(decoded.data==mono.data && decoded.step==2);
        std::printf("PASS_NATIVE_IMAGE_CONTRACT checks=%u\n",checks);
        return 0;
    }
    catch(std::exception const& error)
    {
        std::fprintf(stderr,"FAIL_NATIVE_IMAGE_CONTRACT %s\n",error.what());
        return 1;
    }
}
