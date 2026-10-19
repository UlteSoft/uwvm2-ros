#pragma once
#include "api.h"
#include <functional>

namespace uwvm2::ros_gateway::ros2_compat
{
    class post_set_lease
    {
        std::shared_ptr<void> handle_;
        std::function<void()> remove_;
    public:
        post_set_lease()=default;
        post_set_lease(std::shared_ptr<void> handle,std::function<void()> remove):handle_{std::move(handle)},remove_{std::move(remove)} {}
        post_set_lease(post_set_lease const&)=delete;post_set_lease& operator=(post_set_lease const&)=delete;
        post_set_lease(post_set_lease&& other):handle_{std::move(other.handle_)},remove_{std::move(other.remove_)}
        { other.remove_={}; }
        post_set_lease& operator=(post_set_lease&& other)
        {
            if(this!=&other)
            {
                if(remove_) { try { remove_(); }catch(...) {} }
                handle_=std::move(other.handle_);remove_=std::move(other.remove_);other.remove_={};
            }
            return *this;
        }
        ~post_set_lease() { if(remove_) { try { remove_(); }catch(...) {} } }
        explicit operator bool() const noexcept { return static_cast<bool>(handle_); }
    };
    template<class Node,class Callback> auto post_set_impl(Node const& node,Callback callback,int)
        -> decltype(node->add_post_set_parameters_callback(callback),post_set_lease{})
    {
        auto handle{node->add_post_set_parameters_callback(std::move(callback))};
        return {handle,[node,handle] { node->remove_post_set_parameters_callback(handle.get()); }};
    }
    template<class Node,class Callback> post_set_lease post_set_impl(Node const&,Callback,long) { return {}; }
    template<class Node,class Callback> post_set_lease post_set(Node const& node,Callback callback)
    { return post_set_impl(node,std::move(callback),0); }
}
