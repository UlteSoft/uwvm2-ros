/** @file
 * @brief Opt-in call-local metadata for authenticated service observation pairs.
 * @details Only the dispatcher can associate its confirmed operator grant labels
 * with a current host call. Nothing from the guest payload, native handle, service
 * reply or unrecognized caller is recorded. Nested calls own different frames.
 */
#pragma once
#include <array>
#include <algorithm>
#include <string_view>
namespace uwvm2::imported::ros::scene::service_observation_audit
{
    enum class withdrawal { none, observer, source, both };
    struct labels
    {
        std::array<char,255> observer{},source{};
        std::size_t observer_length{},source_length{};
        withdrawal lost{};
    };
    extern "C++" {inline thread_local labels* current{};}
    class frame
    {
        labels* previous;
        labels state{};
    public:
        explicit frame(bool enabled) noexcept:previous(current){current=enabled?&state:nullptr;}
        frame(frame const&)=delete;
        ~frame(){current=previous;}
    };
    inline void associate(std::string_view observer,std::string_view source) noexcept
    {
        if(!current||observer.empty()||source.empty()||observer.size()>current->observer.size()||
           source.size()>current->source.size())return;
        std::copy(observer.begin(),observer.end(),current->observer.begin());
        std::copy(source.begin(),source.end(),current->source.begin());
        current->observer_length=observer.size();current->source_length=source.size();
    }
}
