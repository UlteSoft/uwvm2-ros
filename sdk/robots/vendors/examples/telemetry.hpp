/** C++20 typed telemetry component; source-only, never compiled here.
 * Message must be a complete freshly generated bounded type with its real
 * roswasmcpp::Codec specialization. No vendor record or native ABI is invented.
 * Context/session labels convey no ROS principal; aliases remain host-selected.
 * Basic/native formatting or fast_io may be bound by the example entry, but
 * the polling path itself needs no WASI file/network/debug/update imports.
 */
#ifndef UWVM_VENDOR_TYPED_TELEMETRY_HPP
#define UWVM_VENDOR_TYPED_TELEMETRY_HPP
#include <roswasm/roswasmcpp.hpp>
#include <cstdint>
#include <span>
#include <string_view>
#include <vector>
namespace uwvm_vendor {
template<class Message> class Telemetry {
    roswasmcpp::Session session_;
    roswasmcpp::Capability* subscription_{};
    std::vector<unsigned char> scratch_;
    uint32_t (*observe_)(Message const&);
    uint32_t status_{},cleanup_{},polls_{},received_{};
    bool started_{},closed_{},denied_{},polling_{};
    uint32_t finish(uint32_t first) {
        if(closed_)return first?first:9;closed_=true;
        uint32_t statuses[1]{};auto report=session_.close(statuses,first);
        cleanup_=report.cleanup_error;return status_=first?first:cleanup_;
    }
public:
    /** maximum is the complete generated wire bound, never a native struct
     * size. A separately reviewed model observer must inspect complete values. */
    Telemetry(roswasmc_backend const& backend,size_t maximum,uint32_t (*observe)(Message const&))
        :session_{backend,1},scratch_(maximum<=32768?maximum:0),observe_{observe} {}
    Telemetry(Telemetry const&)=delete;Telemetry& operator=(Telemetry const&)=delete;
    uint32_t initialize(uint32_t mode,std::string_view alias) {
        if(started_||closed_)return 9;
        if(mode>1||alias.empty()||alias.size()>64||!observe_||scratch_.empty())return 22;
        started_=true;if(auto e=session_.check_abi())return finish(e);
        auto opened=session_.open(alias);
        if(mode==0){denied_=opened.error==13&&session_.size()==0;return finish(denied_?0:opened.error?opened.error:22);}
        if(opened.error)return finish(opened.error);subscription_=opened.owner;
        if(auto e=subscription_->subscribe())return finish(e);return 0;
    }
    /** The caller owns pacing. No asynchronous native callback enters Guest. */
    uint32_t poll_once() {
        if(polling_)return 17;if(!started_||closed_)return 9;if(polls_++>=4096)return finish(110);
        struct Lease {bool& b;explicit Lease(bool& value):b{value}{b=true;}~Lease(){b=false;}} lease{polling_};
        auto result=subscription_->poll(scratch_);if(result.error==11)return 11;
        if(result.error)return finish(result.error);
        if(!result.has_length||result.length>scratch_.size())return finish(22);
        Message value{};
        if(!roswasmcpp::Codec<Message>::decode(std::span<unsigned char const>{scratch_.data(),static_cast<size_t>(result.length)},value))return finish(22);
        if(auto e=observe_(value))return finish(e);++received_;return 0;
    }
    uint32_t close(){return polling_?17:finish(status_);}
    uint32_t received()const{return received_;}
    bool denied_verified()const{return denied_;}
    uint32_t status()const{return status_;}
    uint32_t cleanup_error()const{return cleanup_;}
};
}
#endif
