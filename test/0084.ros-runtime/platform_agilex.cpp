/** @file @brief Closed vendor conversion, native common fencing, stop and exact integer quantization. */
#include <uwvm2/imported/ros/scene/platform_agilex_receiver.h>
#include <cassert>
#include <cfenv>
#include <cstdio>
#include <limits>
namespace s=uwvm2::imported::ros::scene;namespace d=s::devices;namespace p=d::protocol;
namespace a=s::platform_extensions::agilex;using d::error;
static s::buffers::profile profile(std::string_view name)
{s::buffers::profile out;assert(out.identity.assign(name));out.sha256.fill(12);return out;}
static d::scalar_schema schema()
{
    d::scalar_schema out;out.profile=profile("agilex.motion");assert(out.frame.assign("base"));out.count=4;
    for(std::size_t i{};i<4;++i)
    {assert(out.fields[i].name.assign(a::motion_names[i]));out.fields[i].quantity=a::motion_units[i];out.fields[i].minimum=-2;out.fields[i].maximum=2;}
    return out;
}
static d::work work()
{
    d::work out;out.result={100,d::state::running,d::effect::locally_accepted,4,d::reason::none};out.deadline=1050;
    out.controller={9,10,4,{1,11,3,12},1,1100};out.command.count=4;
    out.command.values[0]=1.25;out.command.values[1]=-.5;out.command.values[2]=.25;out.command.values[3]=-1;
    return out;
}
static void closed_codec_and_boundaries()
{
    a::motion_codec codec;a::motion_frame output{99,{1}};auto command=work().command;
    assert(codec.encode(command,output)==error::denied&&output.identifier==99);
    assert(codec.configure(schema())==error::success&&codec.configure(schema())==error::denied);
    assert(codec.encode(command,output)==error::success&&output.identifier==0x111);
    constexpr std::array<std::uint8_t,8> expected{4,226,254,12,0,250,252,24};assert(output.payload==expected);
    a::motion_sample sample;assert(codec.feedback(0x221,output.payload,sample)==error::success);
    for(std::size_t i{};i<4;++i)assert(sample.values[i]==command.values[i]);
    for(std::size_t n{};n<8;++n)assert(codec.feedback(0x221,{output.payload.data(),n},sample)==error::invalid);
    for(auto id:{0x111u,0x80000221u,0x40000221u,0x20000221u,0x222u})assert(codec.feedback(id,output.payload,sample)==error::invalid);
    auto before=output;command.values[1]=std::numeric_limits<double>::quiet_NaN();
    assert(codec.encode(command,output)==error::invalid&&output.payload==before.payload);
    command=work().command;command.values[0]=2.00001;assert(codec.encode(command,output)==error::denied&&output.payload==before.payload);
    command=work().command;command.count=3;assert(codec.encode(command,output)==error::invalid);
    for(unsigned variant{};variant<5;++variant)
    {
        auto changed=schema();
        if(variant==0)changed.fields[0].quantity=d::unit::radian;
        if(variant==1)changed.fields[0].name=changed.fields[1].name;
        if(variant==2)changed.fields[0].minimum=.1;
        if(variant==3)changed.fields[0].maximum=40;
        if(variant==4)changed.count=3;
        a::motion_codec denied;assert(denied.configure(changed)==error::invalid&&denied.encode(work().command,output)==error::denied);
    }
    auto constrained=schema();constrained.fields[2].minimum=constrained.fields[2].maximum=0;
    a::motion_codec differential;assert(differential.configure(constrained)==error::success);
    assert(differential.encode(work().command,output)==error::denied);
    assert((differential.zero_motion(output)==error::success&&output.identifier==0x111&&output.payload==std::array<std::uint8_t,8>{}));
}
static void quantization_is_independent_of_host_fp()
{
    auto original=std::fegetround();
    for(auto rounding:{FE_TONEAREST,FE_DOWNWARD,FE_UPWARD,FE_TOWARDZERO})
    {
        assert(std::fesetround(rounding)==0);
        for(int n=-32000;n<=32000;n+=125)
        {
            // Binary-exact 1/8-SI grid, independent reference and both signs.
            double value=static_cast<double>(n)/1000.;std::int16_t result{};
            assert(a::milliscale(value,result)&&result==n);
        }
        std::int16_t unchanged{123};
        for(auto bits:{UINT64_C(0x7ff0000000000000),UINT64_C(0xfff0000000000000),UINT64_C(0x7ff0000000000001)})
        {assert(std::feclearexcept(FE_ALL_EXCEPT)==0);assert(!a::milliscale(std::bit_cast<double>(bits),unchanged)&&unchanged==123);assert(!std::fetestexcept(FE_ALL_EXCEPT));}
        assert(a::milliscale(std::numeric_limits<double>::denorm_min(),unchanged)&&unchanged==0);
        assert(a::milliscale(-0.,unchanged)&&unchanged==0);
        assert(!a::milliscale(33.,unchanged)&&!a::milliscale(-33.,unchanged));
        // Exact truncation of the represented value differs from rounded double
        // multiplication at decimal cell boundaries; this difference is intentional.
        auto below=std::bit_cast<double>(std::bit_cast<std::uint64_t>(.125)-1);
        assert(a::milliscale(below,unchanged)&&unchanged==124&&std::fegetround()==rounding);
        auto full=schema();for(auto& field:full.fields){field.minimum=-32.768;field.maximum=32.767;}
        a::motion_codec codec;assert(codec.configure(full)==error::success);d::scalar_command input;input.count=4;a::motion_frame frame;
        for(int sign:{-1,1})for(int cell{};cell<32767;++cell)
        {
            // Cell midpoints stay well away from rounding boundaries on every
            // supported ABI. Expected integral values do not use the codec's
            // significand arithmetic or a second implementation of its shifts.
            input.values[0]=sign*(static_cast<double>(cell)+.5)/1000.;
            assert(codec.encode(input,frame)==error::success);
            auto word=static_cast<std::uint16_t>((static_cast<unsigned>(frame.payload[0])<<8)|frame.payload[1]);
            assert(std::bit_cast<std::int16_t>(word)==sign*cell);
        }
        constexpr std::array edges{UINT64_C(0x4040622d0e560419),UINT64_C(0x4040622d0e560418),
                                  UINT64_C(0xc040624dd2f1a9fc),UINT64_C(0xc040624dd2f1a9fb)};
        constexpr std::array expected{32767,32766,-32768,-32767};
        for(std::size_t i{};i<edges.size();++i)
        {
            input.values[0]=std::bit_cast<double>(edges[i]);assert(codec.encode(input,frame)==error::success);
            auto word=static_cast<std::uint16_t>((static_cast<unsigned>(frame.payload[0])<<8)|frame.payload[1]);
            assert(std::bit_cast<std::int16_t>(word)==expected[i]);
            assert(a::milliscale(input.values[0],unchanged)&&unchanged==expected[i]);
        }
    }
    assert(std::fesetround(original)==0);std::feclearexcept(FE_ALL_EXCEPT);
}
static void common_authority_and_stop()
{
    auto selected=schema();auto stop=profile("fixed.zero");auto request=work();a::motion_receiver receiver;
    p::packet frame,install,stopping;p::header header;header.provider=999;a::motion_frame native{999,{}};
    assert(p::command(request,selected,1,frame)==error::success);
    assert(receiver.command(frame.view(),1001,header,native)==error::denied&&header.provider==999&&native.identifier==999);
    assert(receiver.configure(4,selected,stop)==error::success);
    assert(receiver.command(frame.view(),1001,header,native)==error::denied);
    assert(receiver.rearm_after_stopped(4,1)==error::success);
    assert(receiver.command(frame.view(),1001,header,native)==error::denied);
    assert(p::controller(request.controller,selected,stop,1,install)==error::success&&receiver.controller(install.view(),1001,header)==error::success);
    assert(receiver.command(frame.view(),1050,header,native)==error::denied);
    auto corrupt=frame;corrupt.data[corrupt.size-1]^=1;assert(receiver.command(corrupt.view(),1001,header,native)==error::invalid);
    assert(receiver.command(frame.view(),1001,header,native)==error::success&&native.identifier==0x111);
    assert(receiver.command(frame.view(),1001,header,native)==error::conflict);
    request.controller.owner.authority_epoch++;assert(p::command(request,selected,2,frame)==error::success);
    assert(receiver.command(frame.view(),1001,header,native)==error::denied);
    request=work();assert(p::command(request,selected,2,frame)==error::success);
    assert(p::stop({101,d::state::queued,d::effect::locally_accepted,4},stop,1,stopping)==error::success);
    assert((receiver.stop(stopping.view(),header,native)==error::success&&receiver.latched()&&native.payload==std::array<std::uint8_t,8>{}));
    assert(receiver.command(frame.view(),1001,header,native)==error::denied);
    assert(receiver.rearm_after_stopped(4,1)==error::denied&&receiver.rearm_after_stopped(4,2)==error::success);
    assert(receiver.controller(install.view(),1001,header)==error::denied); // Cannot replay old controller epoch/sequence.
    request.controller.epoch++;assert(p::controller(request.controller,selected,stop,2,install)==error::success);
    assert(receiver.controller(install.view(),1001,header)==error::success);
    assert(receiver.expire(1100,native)&&receiver.latched()&&!receiver.expire(1101,native));
    assert(receiver.command(frame.view(),1001,header,native)==error::denied);
    // A translator refusal must not consume a common sequence or alter outputs.
    p::receiver shared;assert(shared.configure(4,selected,stop)==error::success&&shared.rearm_after_stopped(4,1)==error::success);
    request=work();assert(p::controller(request.controller,selected,stop,1,install)==error::success);
    assert(shared.controller(install.view(),1001,header)==error::success&&p::command(request,selected,1,frame)==error::success);
    d::scalar_command decoded;decoded.count=23;header.provider=999;
    assert(shared.command_translate(frame.view(),1001,header,decoded,[](d::scalar_command const&)noexcept{return error::denied;})==error::denied);
    assert(decoded.count==23&&header.provider==999&&shared.command(frame.view(),1001,header,decoded)==error::success&&decoded.count==4);
}
static void command_watchdog_cannot_be_renewed_after_expiry()
{
    auto selected=schema();auto stop=profile("fixed.zero");auto request=work();
    a::motion_receiver receiver;p::packet frame,install;p::header header;a::motion_frame native;
    assert(receiver.configure(4,selected,stop)==error::success&&receiver.rearm_after_stopped(4,1)==error::success);
    assert(p::controller(request.controller,selected,stop,1,install)==error::success&&receiver.controller(install.view(),1001,header)==error::success);
    assert(p::command(request,selected,1,frame)==error::success&&receiver.command(frame.view(),1001,header,native)==error::success);
    assert(!receiver.expire(1049,native));
    // Installing a later controller before expiry cannot extend active motion.
    request.controller.epoch++;request.controller.expires=2000;
    assert(p::controller(request.controller,selected,stop,2,install)==error::success&&receiver.controller(install.view(),1049,header)==error::success);
    request.deadline=1500;assert(p::command(request,selected,2,frame)==error::success);
    auto before=native;header.provider=999;
    assert(receiver.command(frame.view(),1050,header,native)==error::denied&&header.provider==999&&native.payload==before.payload);
    request.controller.epoch++;assert(p::controller(request.controller,selected,stop,3,install)==error::success);
    assert(receiver.controller(install.view(),1050,header)==error::denied);
    // A rejected late call must not consume the event or silence the stop lane.
    assert((receiver.expire(1050,native)&&native.payload==std::array<std::uint8_t,8>{}&&receiver.latched()));
    assert(!receiver.expire(1051,native)&&receiver.command(frame.view(),1051,header,native)==error::denied);
    assert(receiver.rearm_after_stopped(4,2)==error::success&&receiver.controller(install.view(),1051,header)==error::success);
    assert(p::command(request,selected,2,frame)==error::success&&receiver.command(frame.view(),1051,header,native)==error::success);
    // A new command before the old one expires legitimately replaces it.
    request.deadline=1600;assert(p::command(request,selected,3,frame)==error::success&&receiver.command(frame.view(),1499,header,native)==error::success);
    assert(!receiver.expire(1500,native)&&receiver.expire(1600,native));

    p::receiver common;d::scalar_command value;
    request=work();assert(common.configure(4,selected,stop)==error::success&&common.rearm_after_stopped(4,1)==error::success&&common.install_controller(request.controller)==error::success);
    assert(p::command(request,selected,1,frame)==error::success&&common.command(frame.view(),1001,header,value)==error::success);
    request.deadline=1090;assert(p::command(request,selected,2,frame)==error::success);
    assert(common.command_translate(frame.view(),1049,header,value,[](auto const&)noexcept{return error::denied;})==error::denied);
    assert(common.expire(1050)); // Failed translation never extends the watchdog.

    a::motion_receiver lease_only;request=work();
    assert(lease_only.configure(4,selected,stop)==error::success&&lease_only.rearm_after_stopped(4,1)==error::success);
    assert(p::controller(request.controller,selected,stop,1,install)==error::success&&lease_only.controller(install.view(),1001,header)==error::success);
    request.controller.epoch++;request.controller.expires=2000;
    assert(p::controller(request.controller,selected,stop,2,install)==error::success);
    assert(lease_only.controller(install.view(),1100,header)==error::denied&&lease_only.expire(1100,native));
}
int main()
{closed_codec_and_boundaries();quantization_is_independent_of_host_fp();common_authority_and_stop();command_watchdog_cannot_be_renewed_after_expiry();std::puts("PASS AgileX exact motion codec and common authority fencing");}
