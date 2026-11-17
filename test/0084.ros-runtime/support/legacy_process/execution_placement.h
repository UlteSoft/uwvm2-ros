// Test-only historical process owner. Never include or link into the VM product.
/** @file
 * @brief Trusted CPU and NUMA placement for isolated native providers.
 * @details Parsing is inert and uses recoverable fast_io storage. Preparation
 * resolves a CPU subset against the preparing thread's actual allowed mask;
 * a requested NUMA set must be a subset of the kernel's allowed memory nodes.
 * The post-fork child applies and verifies CPU placement before executing any
 * provider code. It never changes the VM's affinity or moves a cgroup. A changed
 * cpuset, unsupported policy or refused syscall fails closed, without fallback.
 *
 * Affinity selects logical CPUs, not exclusive physical cores, CPU bandwidth or
 * real-time scheduling. NUMA policy governs future eligible allocations; it does
 * not relocate sealed files, shared pages or already resident VM buffers. Those
 * distinctions must remain visible in deployment and latency qualification.
 */
#pragma once
#include <uwvm2/imported/ros/runtime/resources.h>
#include <uwvm2/utils/container/standard.h>
#include <algorithm>
#include <charconv>
#include <climits>
#include <span>
#include <string_view>
#if defined(__linux__)
#include <cerrno>
#include <linux/mempolicy.h>
#include <sys/syscall.h>
#include <poll.h>
#include <unistd.h>
#endif

namespace uwvm2::imported::ros::scene::execution_placement
{
    using error=resources::error;
    using word=unsigned long;
    inline constexpr std::size_t word_bits=sizeof(word)*CHAR_BIT;
    // A bounded metadata allocation, not a 64-CPU/CPU_SETSIZE topology ceiling.
    inline constexpr std::size_t maximum_mask_bytes=1024*1024;
    using mask=::uwvm2::utils::container::std_vector<word>;
    enum class memory_policy:std::uint8_t{inherit,local,bind,interleave};
    /** @brief An empty CPU set inherits the preparing thread's allowed placement.
     * @details An empty memory set is valid only for inherit/local. Explicit
     * placement grants no device, network, model or scheduling authority.
     */
    struct policy{mask cpus,nodes;memory_policy memory{};};
    inline bool nonempty(std::span<word const> value)noexcept
    {for(auto bits:value)if(bits)return true;return false;}
    inline bool valid(policy const& value)noexcept
    {
        if(value.cpus.size()>maximum_mask_bytes/sizeof(word)||value.nodes.size()>maximum_mask_bytes/sizeof(word)||
           value.memory>memory_policy::interleave||(!value.cpus.empty()&&!value.cpus.back()))return false;
        return value.memory==memory_policy::bind||value.memory==memory_policy::interleave?!value.nodes.empty()&&value.nodes.back():value.nodes.empty();
    }
    /** @brief Decode ascending decimal IDs/ranges without duplicates or hidden coercions.
     * @details Empty text, signs, whitespace, inverted/overlapping ranges,
     * overflow and a trailing comma reject. Output is unchanged on failure.
     * Text is bounded before allocation; IDs above the supported metadata bound
     * return too_large, never a silently truncated CPU or NUMA set.
     */
    inline error parse_mask(std::string_view text,mask& output)noexcept
    {
        if(text.empty()||text.size()>4095)return error::invalid;
        auto scan=[&](auto&& visit)noexcept
        {
            std::size_t at{};std::uint64_t previous{};bool first=true;
            while(at<text.size())
            {
                auto number=[&](std::uint64_t& value)noexcept
                {
                    if(at==text.size()||text[at]<'0'||text[at]>'9')return false;
                    auto result=std::from_chars(text.data()+at,text.data()+text.size(),value);
                    if(result.ec!=std::errc{})return false;
                    at=static_cast<std::size_t>(result.ptr-text.data());return true;
                };
                std::uint64_t begin{},end{};if(!number(begin))return error::invalid;end=begin;
                if(at<text.size()&&text[at]=='-'){++at;if(!number(end))return error::invalid;}
                if(end<begin||(!first&&begin<=previous))return error::invalid;
                if(end>=maximum_mask_bytes*CHAR_BIT)return error::too_large;
                visit(static_cast<std::size_t>(begin),static_cast<std::size_t>(end));first=false;previous=end;
                if(at==text.size())break;
                if(text[at++]!=','||at==text.size())return error::invalid;
            }
            return error::success;
        };
        std::size_t highest{};auto status=scan([&](auto,auto end){highest=end;});if(status!=error::success)return status;
        try
        {
            mask value(highest/word_bits+1);
            status=scan([&](auto begin,auto end){for(auto i=begin;i<=end;++i)value[i/word_bits]|=word{1}<<(i%word_bits);});
            if(status!=error::success)return status;
            output=std::move(value);return error::success;
        }
        catch(...){return error::no_memory;}
    }
    /** @brief Parse one complete deployment choice with no native probes.
     * @param cpus `inherit` or an ascending CPU ID/range list.
     * @param memory `inherit`, `local`, `bind` or `interleave`.
     * @param nodes `none` for inherit/local, otherwise an ascending node list.
     */
    inline error parse(std::string_view cpus,std::string_view memory,std::string_view nodes,policy& output)noexcept
    {
        policy value;auto status=error::success;
        if(cpus!="inherit"){status=parse_mask(cpus,value.cpus);if(status!=error::success)return status;}
        if(memory=="inherit")value.memory=memory_policy::inherit;
        else if(memory=="local")value.memory=memory_policy::local;
        else if(memory=="bind")value.memory=memory_policy::bind;
        else if(memory=="interleave")value.memory=memory_policy::interleave;
        else return error::invalid;
        if(value.memory==memory_policy::bind||value.memory==memory_policy::interleave)
        {status=parse_mask(nodes,value.nodes);if(status!=error::success)return status;}
        else if(nodes!="none")return error::invalid;
        output=std::move(value);return error::success;
    }
    inline bool subset(std::span<word const> selected,std::span<word const> allowed)noexcept
    {for(std::size_t i{};i<selected.size();++i)if(selected[i]&(i<allowed.size()?~allowed[i]:~word{}))return false;return true;}
    /** @brief Parent-prepared masks; application performs only nonallocating syscalls.
     * @details The object belongs to one preparing process. Only its direct
     * post-fork child may apply it; ordinary calls in the VM fail before changing
     * policy. The sandbox's syscall filter separately denies subsequent affinity
     * and NUMA-policy changes by provider code. No external topology file is
     * trusted to authorize a CPU/node absent from the kernel's allowed masks.
     */
    class resolved
    {
        mask cpus_,nodes_,scratch_,node_scratch_;memory_policy memory_{};bool ready_{};
#if defined(__linux__)
        ::pid_t owner_{};
        static error allowed(bool memory,mask& output)
        {
            for(std::size_t bytes=sizeof(word);bytes<=maximum_mask_bytes;bytes*=2)
            {
                mask value(bytes/sizeof(word));
                long result{};
                if(memory)
                {
#if defined(SYS_get_mempolicy)
                    result=::syscall(SYS_get_mempolicy,nullptr,value.data(),bytes*CHAR_BIT,nullptr,MPOL_F_MEMS_ALLOWED);
#else
                    return error::unsupported;
#endif
                }
                else result=::syscall(SYS_sched_getaffinity,0,bytes,value.data());
                if(result>=0){if(!nonempty(value))return error::denied;output=std::move(value);return error::success;}
                if(errno==EINVAL)continue;
                return errno==ENOSYS?error::unsupported:errno==EPERM||errno==EACCES?error::denied:error::io;
            }
            return error::too_large;
        }
#endif
    public:
        /** @brief Resolve once in the parent; failure leaves no usable placement. */
        error prepare(policy const& selected)noexcept
        {
            if(ready_)return error::conflict;
            if(!valid(selected))return error::invalid;
#if defined(__linux__)
            try
            {
                mask cpus,nodes;auto status=allowed(false,cpus);if(status!=error::success)return status;
                if(!selected.cpus.empty())
                {
                    if(!subset(selected.cpus,cpus))return error::denied;
                    // Preserve the kernel-sized extent for later verification.
                    std::fill(cpus.begin(),cpus.end(),0);std::copy(selected.cpus.begin(),selected.cpus.end(),cpus.begin());
                }
                if(selected.memory==memory_policy::bind||selected.memory==memory_policy::interleave)
                {
                    status=allowed(true,nodes);if(status!=error::success)return status;
                    if(!subset(selected.nodes,nodes))return error::denied;
                    std::fill(nodes.begin(),nodes.end(),0);std::copy(selected.nodes.begin(),selected.nodes.end(),nodes.begin());
                }
                mask scratch(cpus.size()),node_scratch(nodes.size());
                cpus_=std::move(cpus);nodes_=std::move(nodes);scratch_=std::move(scratch);node_scratch_=std::move(node_scratch);
                memory_=selected.memory;owner_=::getpid();ready_=true;return error::success;
            }
            catch(...){return error::no_memory;}
#else
            return error::unsupported;
#endif
        }
        std::span<word const> cpus()const noexcept{return cpus_;}
        std::span<word const> nodes()const noexcept{return nodes_;}
        memory_policy memory()const noexcept{return memory_;}
        /** @brief Apply in the direct child before exec; partial failure must exit the child.
         * @details This is a launch gate, not a reversible runtime reconfiguration
         * API. The caller must not execute provider code after false. CPU hotplug
         * or cpuset contraction is detected by exact post-application readback.
         */
        bool apply()noexcept{return apply_child(-1,0);}
        /** @brief Apply the same policy in a newly born PID namespace init.
         * @details Only the fixed raw-clone child supplies its inherited pinned
         * owner and the preparing owner ID. The parent is invisible in this PID
         * namespace, so numerical getppid equality cannot establish provenance.
         * This check does not relax affinity, allowed-node or readback checks.
         * Failure must exit before any external image executes.
         */
        bool apply_namespace_child(int parent_identity,std::int64_t preparing_owner)noexcept
        {return parent_identity>=0&&apply_child(parent_identity,preparing_owner);}
    private:
        bool apply_child(int parent_identity,std::int64_t preparing_owner)noexcept
        {
#if defined(__linux__)
            if(!ready_)return false;
            if(parent_identity>=0)
            {
                if(preparing_owner!=owner_||::getpid()!=1||::getppid()!=0)return false;
                pollfd parent{parent_identity,POLLIN,0};if(::poll(&parent,1,0)!=0)return false;
            }
            else if(::getpid()==owner_||::getppid()!=owner_)return false;
            std::fill(scratch_.begin(),scratch_.end(),0);auto bytes=cpus_.size()*sizeof(word);
            if(::syscall(SYS_sched_getaffinity,0,bytes,scratch_.data())<0||!subset(cpus_,scratch_))return false;
            if(::syscall(SYS_sched_setaffinity,0,bytes,cpus_.data()))return false;
            std::fill(scratch_.begin(),scratch_.end(),0);
            if(::syscall(SYS_sched_getaffinity,0,bytes,scratch_.data())<0||cpus_!=scratch_)return false;
            if(memory_==memory_policy::inherit)return true;
#if defined(SYS_set_mempolicy) && defined(SYS_get_mempolicy)
            int mode=memory_==memory_policy::local?MPOL_LOCAL:memory_==memory_policy::bind?MPOL_BIND:MPOL_INTERLEAVE;
            auto data=nodes_.empty()?nullptr:nodes_.data();auto bits=nodes_.size()*word_bits;
            if(data)
            {
                std::fill(node_scratch_.begin(),node_scratch_.end(),0);
                if(::syscall(SYS_get_mempolicy,nullptr,node_scratch_.data(),bits,nullptr,MPOL_F_MEMS_ALLOWED)||!subset(nodes_,node_scratch_))return false;
                mode|=MPOL_F_STATIC_NODES;
            }
            if(::syscall(SYS_set_mempolicy,mode,data,bits))return false;
            int actual{};std::fill(node_scratch_.begin(),node_scratch_.end(),0);
            if(::syscall(SYS_get_mempolicy,&actual,data?node_scratch_.data():nullptr,bits,nullptr,0)||actual!=mode)return false;
            return nodes_==node_scratch_;
#else
            return false;
#endif
#else
            (void)parent_identity;(void)preparing_owner;return false;
#endif
        }
    };
}
