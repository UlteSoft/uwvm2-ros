#pragma once
#include <uwvm2/imported/ros/ipc/parameter.h>
#include <algorithm>
#include <initializer_list>
#include <set>
#include <string>
#include <string_view>

namespace uwvm2::ros_gateway::bounded_text
{
    namespace wire=uwvm2::imported::ros::ipc;

    // Preflight borrowed fragments before allocating the canonical wire result.
    // Native ROS builds use C++17; no intermediate formatted string is needed.
    inline wire::call_result result(std::uint64_t maximum,std::initializer_list<std::string_view> fragments)
    {
        maximum=std::min<std::uint64_t>(maximum,wire::max_payload_bytes);
        if(maximum<4) { return {90,{}}; }
        std::size_t size{4};
        for(auto fragment:fragments)
        {
            if(fragment.size()>maximum-size) { return {90,{}}; }
            size+=fragment.size();
        }
        for(auto fragment:fragments)
        {
            if(!wire::parameter_payload_utf8(reinterpret_cast<std::uint8_t const*>(fragment.data()),fragment.size())) { return {22,{}}; }
        }
        std::vector<std::uint8_t> bytes;bytes.reserve(size);wire::put(bytes,size-4,4);
        for(auto fragment:fragments) { if(!fragment.empty()) { bytes.insert(bytes.end(),fragment.begin(),fragment.end()); } }
        return {0,std::move(bytes)};
    }

    class sorted_rows
    {
        std::uint64_t maximum_{};
        std::size_t size_{};
        std::uint32_t error_{};
        std::set<std::string> rows_{};
    public:
        explicit sorted_rows(std::uint64_t maximum) noexcept:
            maximum_{std::min<std::uint64_t>(maximum,wire::max_payload_bytes)},error_{maximum_<4?90u:0u} {}

        bool add(std::string_view name,std::string_view type={})
        {
            if(error_) { return false; }
            // Bound each temporary before copying. Retained unique rows together
            // fit the same result quota; duplicates consume no additional quota.
            auto remaining{maximum_-4};std::size_t size{};
            for(auto fragment:{name,type,std::string_view{"\n"},type.empty()?std::string_view{}:std::string_view{"\t"}})
            {
                if(fragment.size()>remaining-size) { error_=90;return false; }
                size+=fragment.size();
            }
            std::string row;row.reserve(size);row.append(name);
            if(!type.empty()) { row+='\t';row.append(type); }row+='\n';
            if(rows_.find(row)!=rows_.end()) { return true; }
            if(size>remaining-size_) { error_=90;return false; }
            rows_.insert(std::move(row));size_+=size;return true;
        }
        bool empty() const noexcept { return rows_.empty(); }
        std::uint32_t error() const noexcept { return error_; }
        std::size_t text_bytes() const noexcept { return size_; }
        wire::call_result result() const
        {
            if(error_) { return {error_,{}}; }
            for(auto const& row:rows_)
            {
                if(!wire::parameter_payload_utf8(reinterpret_cast<std::uint8_t const*>(row.data()),row.size())) { return {22,{}}; }
            }
            std::vector<std::uint8_t> bytes;bytes.reserve(4+size_);wire::put(bytes,size_,4);
            for(auto const& row:rows_) { bytes.insert(bytes.end(),row.begin(),row.end()); }
            return {0,std::move(bytes)};
        }
    };
}
