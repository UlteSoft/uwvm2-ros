/** SDK-free C++17 ownership model for the production legacy Action lease.
 *
 * No ROS context, native endpoint, executor or background ROS thread is
 * initialized. Eight cases exercise actual construction/registration and
 * destruction through the same generic helper used by the native adapter.
 * A model success is not a Foxy/native Action delivery qualification.
 */
#include "../../gateways/ros2/compat/action_client_lease.h"
#include <array>
#include <cstdlib>
#include <stdexcept>
#include <thread>
#include <vector>
#ifdef NDEBUG
#error "The ownership model requires checks enabled"
#endif
namespace
{
    void require(bool value){if(!value) {std::abort();}}
    struct options
    {
        std::array<unsigned,5> channels{{1,2,3,4,5}};
        bool throw_constructor{};
    };
    struct client
    {
        inline static std::atomic<unsigned> live{},destroyed{};
        options selected;
        client(int base,int graph,int logging,std::string const& name,options const& requested):selected{requested}
        {
            require(base==10&&graph==20&&logging==30&&name=="/selected");
            if(requested.throw_constructor) {throw std::runtime_error("constructor refusal");}
            ++live;
        }
        ~client(){--live;++destroyed;}
    };
    struct waitables
    {
        std::atomic<unsigned> added{},removed{};
        std::weak_ptr<client> selected;
        bool throw_add{},throw_after_add{},throw_remove{};
        void add_waitable(std::shared_ptr<client> const& value,std::nullptr_t)
        {
            ++added;if(throw_add) {throw std::runtime_error("registration refusal");}
            selected=value;if(throw_after_add) {throw std::runtime_error("partial registration refusal");}
        }
        void remove_waitable(std::shared_ptr<client> const& value,std::nullptr_t)
        {
            require(value&&client::live==1);++removed;
            if(throw_remove) {throw std::runtime_error("retirement refusal");}
            selected.reset();
        }
    };
    struct node
    {
        std::shared_ptr<waitables> scope=std::make_shared<waitables>();
        int get_node_base_interface() const{return 10;}
        int get_node_graph_interface() const{return 20;}
        int get_node_logging_interface() const{return 30;}
        std::shared_ptr<waitables> get_node_waitables_interface() const{return scope;}
    };
    auto create(std::shared_ptr<node> const& owner,options const& selected={})
    {return uwvm2::ros_gateway::ros2_compat::legacy_action_client<client>(owner,"/selected",selected);}
}
int main()
{
    // 1. Preserve every selected channel and register/unregister exactly once.
    {
        auto owner=std::make_shared<node>();options selected;selected.channels={{19,23,29,31,37}};
        auto value=create(owner,selected);require(value->selected.channels==selected.channels);
        require(owner->scope->added==1&&owner->scope->removed==0&&client::live==1);
        value.reset();require(owner->scope->removed==1&&client::live==0);
    }
    // 2. Shared ownership copies cannot unregister a still-owned native client.
    {
        auto owner=std::make_shared<node>();auto first=create(owner);auto second=first;
        first.reset();require(owner->scope->removed==0&&client::live==1);
        second.reset();require(owner->scope->removed==1&&client::live==0);
    }
    // 3. Failed typed construction does not register or unregister anything.
    {
        auto owner=std::make_shared<node>();options selected;selected.throw_constructor=true;bool failed{};
        try{auto value=create(owner,selected);(void)value;}catch(std::runtime_error const&){failed=true;}
        require(failed&&client::live==0&&owner->scope->added==0&&owner->scope->removed==0);
    }
    // 4. Both early and partially completed registration roll back once.
    {
        auto owner=std::make_shared<node>();owner->scope->throw_add=true;bool failed{};
        try{auto value=create(owner);(void)value;}catch(std::runtime_error const&){failed=true;}
        require(failed&&client::live==0&&owner->scope->added==1&&owner->scope->removed==1);
        owner->scope->throw_add=false;owner->scope->throw_after_add=true;failed=false;
        try{auto value=create(owner);(void)value;}catch(std::runtime_error const&){failed=true;}
        require(failed&&client::live==0&&owner->scope->added==2&&owner->scope->removed==2&&owner->scope->selected.expired());
    }
    // 5. Retirement exceptions never escape a deleter or leak the native object.
    {
        auto owner=std::make_shared<node>();owner->scope->throw_remove=true;auto value=create(owner);
        value.reset();require(client::live==0&&owner->scope->removed==1);
    }
    // 6. A surviving client must not keep its Node or waitables owner alive.
    {
        auto owner=std::make_shared<node>();std::weak_ptr<node> old_node=owner;
        std::weak_ptr<waitables> old_waitables=owner->scope;auto value=create(owner);owner.reset();
        require(old_node.expired()&&old_waitables.expired()&&client::live==1);
        value.reset();require(client::live==0);
    }
    // 7. An independently retained public waitables owner can still retire it.
    {
        auto owner=std::make_shared<node>();auto scope=owner->scope;auto value=create(owner);owner.reset();
        value.reset();require(scope->removed==1&&client::live==0);
    }
    // 8. Concurrent copies retire one registered native object exactly once.
    {
        auto owner=std::make_shared<node>();auto value=create(owner);std::vector<std::thread> threads;
        for(unsigned i=0;i<8;++i) {threads.emplace_back([copy=value]() mutable {copy.reset();});}
        value.reset();for(auto& thread:threads) {thread.join();}
        require(owner->scope->added==1&&owner->scope->removed==1&&client::live==0);
    }
    require(client::destroyed==8);
}
