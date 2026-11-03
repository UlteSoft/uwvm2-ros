// SPDX-License-Identifier: Apache-2.0
#pragma once
#include "wire.h"
#include <algorithm>
#include <initializer_list>
#include <limits>
#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>
#include <fast_io_crypto.h>
#if defined(__linux__)
# include <fcntl.h>
# include <sys/stat.h>
# include <unistd.h>
# include <cerrno>
#endif

namespace uwvm2::imported::ros::portable
{
    inline digest hash_bytes(::std::string_view bytes) noexcept
    {
        digest result{};::fast_io::sha256_context hash{};
        auto begin{reinterpret_cast<::std::byte const*>(bytes.data())};
        hash.update(begin,begin+bytes.size());hash.do_final();
        hash.digest_to_byte_ptr(reinterpret_cast<::std::byte*>(result.data()));return result;
    }
    inline bool parse_digest(::std::string_view text,digest& output) noexcept
    {
        if(text.size()!=64u) { return false; }digest value{};
        for(::std::size_t i{};i!=32u;++i)
        {
            unsigned byte{};
            for(unsigned j{};j!=2u;++j)
            {
                auto c{text[i*2u+j]};unsigned n{};
                if(c>='0'&&c<='9') { n=static_cast<unsigned>(c-'0'); }
                else if(c>='a'&&c<='f') { n=static_cast<unsigned>(c-'a'+10); }
                else { return false; }byte=byte*16u+n;
            }
            value[i]=static_cast<::std::uint8_t>(byte);
        }
        output=value;return true;
    }

    namespace deployment_detail
    {
        inline bool utf8(::std::string_view text) noexcept
        {
            for(::std::size_t i{};i<text.size();)
            {
                auto first{static_cast<unsigned char>(text[i++])};
                if(first<0x80u) { if(first==0u) { return false; }continue; }
                unsigned count{},minimum{},value{};
                if(first>=0xc2u&&first<=0xdfu) { count=1;minimum=0x80;value=first&31u; }
                else if(first>=0xe0u&&first<=0xefu) { count=2;minimum=0x800;value=first&15u; }
                else if(first>=0xf0u&&first<=0xf4u) { count=3;minimum=0x10000;value=first&7u; }
                else { return false; }
                if(count>text.size()-i) { return false; }
                for(unsigned j{};j!=count;++j)
                { auto c{static_cast<unsigned char>(text[i++])};if((c&0xc0u)!=0x80u) { return false; }value=(value<<6u)|(c&63u); }
                if(value<minimum||value>0x10ffffu||(value>=0xd800u&&value<=0xdfffu)) { return false; }
            }
            return true;
        }
        struct value
        {
            enum class kind { object,array,string,integer } type{};
            ::std::string text{};
            ::std::uint64_t magnitude{};bool negative{};
            ::std::map<::std::string,value,::std::less<>> members{};
            ::std::vector<value> elements{};
            value const* at(::std::string_view key) const noexcept
            { auto i{members.find(key)};return type==kind::object&&i!=members.end()?::std::addressof(i->second):nullptr; }
        };
        // Closed configuration dialect: objects, arrays, UTF-8 strings and
        // integer lexemes only. No floating point conversion, duplicate keys,
        // comments, embedded NUL, trailing input or unbounded recursion.
        class parser
        {
            ::std::string_view input_;::std::size_t at_{},nodes_{};
            void space() noexcept { while(at_<input_.size()&&(input_[at_]==' '||input_[at_]=='\t'||input_[at_]=='\r'||input_[at_]=='\n')) { ++at_; } }
            bool take(char c) noexcept { space();if(at_==input_.size()||input_[at_]!=c) { return false; }++at_;return true; }
            bool hex4(unsigned& n) noexcept
            {
                if(input_.size()-at_<4u) { return false; }n=0;
                for(unsigned i{};i!=4u;++i)
                {
                    auto c{input_[at_++]};unsigned d{};
                    if(c>='0'&&c<='9') { d=static_cast<unsigned>(c-'0'); }
                    else if(c>='a'&&c<='f') { d=static_cast<unsigned>(c-'a'+10); }
                    else if(c>='A'&&c<='F') { d=static_cast<unsigned>(c-'A'+10); }
                    else { return false; }n=n*16u+d;
                }
                return true;
            }
            bool string(::std::string& result)
            {
                if(!take('"')) { return false; }result.clear();
                while(at_<input_.size())
                {
                    auto c{static_cast<unsigned char>(input_[at_++])};
                    if(c=='"') { return result.size()<=4096u&&utf8(result); }
                    if(c<0x20u) { return false; }
                    if(c!='\\') { result.push_back(static_cast<char>(c));continue; }
                    if(at_==input_.size()) { return false; }auto escaped{input_[at_++]};
                    switch(escaped)
                    {
                        case '"':case '\\':case '/':result.push_back(escaped);break;
                        case 'b':result.push_back('\b');break;case 'f':result.push_back('\f');break;
                        case 'n':result.push_back('\n');break;case 'r':result.push_back('\r');break;case 't':result.push_back('\t');break;
                        case 'u':
                        {
                            unsigned n{};if(!hex4(n)||!n) { return false; }
                            if(n>=0xd800u&&n<=0xdbffu)
                            {
                                if(input_.size()-at_<2u||input_[at_]!='\\'||input_[at_+1u]!='u') { return false; }at_+=2u;
                                unsigned low{};if(!hex4(low)||low<0xdc00u||low>0xdfffu) { return false; }
                                n=0x10000u+((n-0xd800u)<<10u)+(low-0xdc00u);
                            }
                            else if(n>=0xdc00u&&n<=0xdfffu) { return false; }
                            if(n<0x80u) { result.push_back(static_cast<char>(n)); }
                            else
                            {
                                if(n<0x800u) { result.push_back(static_cast<char>(0xc0u|(n>>6u))); }
                                else
                                {
                                    if(n<0x10000u) { result.push_back(static_cast<char>(0xe0u|(n>>12u))); }
                                    else { result.push_back(static_cast<char>(0xf0u|(n>>18u)));result.push_back(static_cast<char>(0x80u|((n>>12u)&63u))); }
                                    result.push_back(static_cast<char>(0x80u|((n>>6u)&63u)));
                                }
                                result.push_back(static_cast<char>(0x80u|(n&63u)));
                            }
                            break;
                        }
                        default:return false;
                    }
                    if(result.size()>4096u) { return false; }
                }
                return false;
            }
            bool item(value& out,unsigned depth)
            {
                space();if(depth>8u||++nodes_>2048u||at_==input_.size()) { return false; }
                auto c{input_[at_]};
                if(c=='"') { out.type=value::kind::string;return string(out.text); }
                if(c=='{'||c=='[')
                {
                    bool object{c=='{'};out.type=object?value::kind::object:value::kind::array;++at_;
                    if(take(object?'}':']')) { return true; }
                    do
                    {
                        ::std::string key{};if(object&&(!string(key)||!take(':'))) { return false; }
                        value child{};if(!item(child,depth+1u)) { return false; }
                        if(object) { if(!out.members.emplace(::std::move(key),::std::move(child)).second) { return false; } }
                        else { out.elements.push_back(::std::move(child)); }
                        if(take(object?'}':']')) { return true; }
                    } while(take(','));return false;
                }
                out.type=value::kind::integer;out.negative=c=='-';if(out.negative) { ++at_; }
                auto start{at_};
                while(at_<input_.size()&&input_[at_]>='0'&&input_[at_]<='9')
                {
                    auto digit{static_cast<unsigned>(input_[at_++]-'0')};
                    if(out.magnitude>((::std::numeric_limits<::std::uint64_t>::max)()-digit)/10u) { return false; }
                    out.magnitude=out.magnitude*10u+digit;
                }
                return at_!=start&&!(at_-start>1u&&input_[start]=='0')&&!(out.negative&&!out.magnitude);
            }
        public:
            explicit parser(::std::string_view input) noexcept:input_{input}{}
            bool parse(value& output)
            { if(input_.empty()||input_.size()>32768u||!utf8(input_)) { return false; }value candidate{};if(!item(candidate,0)) { return false; }space();if(at_!=input_.size()) { return false; }output=::std::move(candidate);return true; }
        };
        inline bool keys(value const& v,::std::initializer_list<::std::string_view> names) noexcept
        { if(v.type!=value::kind::object||v.members.size()!=names.size()) { return false; }for(auto key:names) { if(!v.at(key)) { return false; } }return true; }
        inline bool text(value const& object,::std::string_view key,::std::string& out,::std::size_t maximum=1024u)
        { auto v{object.at(key)};if(!v||v->type!=value::kind::string||v->text.empty()||v->text.size()>maximum) { return false; }out=v->text;return true; }
        inline bool equal(value const& object,::std::string_view key,::std::string_view expected) noexcept
        { auto v{object.at(key)};return v&&v->type==value::kind::string&&v->text==expected; }
        inline bool number(value const& object,::std::string_view key,::std::uint64_t& out,::std::uint64_t maximum=UINT64_MAX) noexcept
        { auto v{object.at(key)};if(!v||v->type!=value::kind::integer||v->negative||v->magnitude>maximum) { return false; }out=v->magnitude;return true; }
        inline bool position(value const& object,::std::string_view key,::std::int64_t& out) noexcept
        { auto v{object.at(key)};if(!v||v->type!=value::kind::integer||v->magnitude>1000000000u) { return false; }out=static_cast<::std::int64_t>(v->magnitude);if(v->negative) { out=-out; }return true; }
        inline bool hex(value const& object,::std::string_view key,digest& out) noexcept
        { auto v{object.at(key)};return v&&v->type==value::kind::string&&parse_digest(v->text,out); }
        inline bool name(::std::string_view value,bool absolute=false) noexcept
        {
            if(value.empty()||value.size()>255u||value.back()=='/'||(absolute&&value.front()!='/')||value.find("//")!=value.npos) { return false; }
            for(auto c:value) { if(!((c>='A'&&c<='Z')||(c>='a'&&c<='z')||(c>='0'&&c<='9')||c=='_'||(absolute&&c=='/'))) { return false; } }
            return true;
        }
        inline error read_exact_file(::std::string const& path,digest const& expected,::std::string& output)
        {
#if defined(__linux__)
            if(path.empty()||path.front()!='/'||path.size()>4096u||path.find('\0')!=path.npos) { return error::malformed; }
            int fd{::open(path.c_str(),O_RDONLY|O_CLOEXEC|O_NOFOLLOW|O_NONBLOCK)};if(fd<0) { return error::unavailable; }
            struct close_fd { int fd;~close_fd(){::close(fd);} } guard{fd};struct stat before{},after{};
            if(::fstat(fd,&before)!=0||!S_ISREG(before.st_mode)||before.st_size<=0||before.st_size>32768) { return error::malformed; }
            ::std::string bytes(static_cast<::std::size_t>(before.st_size),'\0');::std::size_t at{};
            while(at!=bytes.size())
            { auto n{::pread(fd,bytes.data()+at,bytes.size()-at,static_cast<off_t>(at))};if(n<0&&errno==EINTR) { continue; }if(n<=0) { return error::unavailable; }at+=static_cast<::std::size_t>(n); }
            char extra{};
            if(::pread(fd,&extra,1,before.st_size)!=0||::fstat(fd,&after)!=0||before.st_dev!=after.st_dev||before.st_ino!=after.st_ino||
               before.st_size!=after.st_size||before.st_mtim.tv_sec!=after.st_mtim.tv_sec||before.st_mtim.tv_nsec!=after.st_mtim.tv_nsec||
               before.st_ctim.tv_sec!=after.st_ctim.tv_sec||before.st_ctim.tv_nsec!=after.st_ctim.tv_nsec||hash_bytes(bytes)!=expected) { return error::stale; }
            output=::std::move(bytes);return error::ok;
#else
            (void)path;(void)expected;(void)output;return error::unsupported;
#endif
        }
    }

    struct joint_deployment
    {
        descriptor mapping{};
        digest adapter_sha256{},policy_sha256{},wasm_sha256{},native_provenance{},native_executable{};
        ::std::uint64_t policy_epoch{},native_generation{};
        ::std::uint32_t native_uid{};
        ::std::string alias{},module{},socket{},platform_id{},official_repository{},official_commit{},official_mode{};
        ::std::array<::std::string,max_joints> native_joints{};
        ::std::string state_topic{},position_action{},controller_service{};
        ::std::string controller_type{"joint_trajectory_controller/JointTrajectoryController"};
    };

    inline error parse_joint_deployment(::std::string_view adapter_bytes,::std::string_view policy_bytes,joint_deployment& output)
    {
        using namespace deployment_detail;value adapter{},policy{};
        if(!parser{adapter_bytes}.parse(adapter)||!parser{policy_bytes}.parse(policy)) { return error::malformed; }
        bool const explicit_controller{equal(adapter,"contract_version","0.1.1")};
        if(!keys(adapter,{"format","contract_version","platform_id","execution_class","capability","official_source","ros","frame","resources","joints","policy_sha256","mapping_epoch"})||
           !equal(adapter,"format","uwvm-portable-robot-adapter-v1")||(!equal(adapter,"contract_version","0.1.0")&&!explicit_controller)||
           !equal(adapter,"execution_class","software_simulation")||!equal(adapter,"frame","joint_space")) { return error::malformed; }
        if(!equal(adapter,"capability","joint.position.bounded.v1")) { return error::unsupported; }
        if(!keys(policy,{"format","alias","module","capability","wasm_sha256","policy_epoch","rights","native","resources"})||
           !equal(policy,"format","uwvm-portable-robot-grant-v1")||!equal(policy,"capability","joint.position.bounded.v1")) { return error::malformed; }
        joint_deployment result{};result.adapter_sha256=hash_bytes(adapter_bytes);result.policy_sha256=hash_bytes(policy_bytes);
        digest expected_policy{};::std::uint64_t rights{},uid{};
        if(!hex(adapter,"policy_sha256",expected_policy)||expected_policy!=result.policy_sha256||!hex(policy,"wasm_sha256",result.wasm_sha256)||
           !number(adapter,"mapping_epoch",result.mapping.epoch)||!result.mapping.epoch||!number(policy,"policy_epoch",result.policy_epoch)||!result.policy_epoch||
           !number(policy,"rights",rights,all_rights)||rights!=all_rights||!text(policy,"alias",result.alias,255)||!name(result.alias)||!text(policy,"module",result.module,255)||
           !text(adapter,"platform_id",result.platform_id,128)) { return error::denied; }
        result.mapping.rights=static_cast<::std::uint32_t>(rights);result.mapping.mapping=result.adapter_sha256;
        auto native{policy.at("native")};
        if(!native||!keys(*native,{"socket","uid","executable_sha256","provenance_sha256","generation"})||
           !text(*native,"socket",result.socket,107)||result.socket.front()!='/'||result.socket.find("..")!=result.socket.npos||
           !number(*native,"uid",uid,4294967294u)||!hex(*native,"executable_sha256",result.native_executable)||
           !hex(*native,"provenance_sha256",result.native_provenance)||!number(*native,"generation",result.native_generation)||!result.native_generation) { return error::malformed; }
        result.native_uid=static_cast<::std::uint32_t>(uid);
        auto official{adapter.at("official_source")};auto ros{adapter.at("ros")};::std::string ignored{};
        if(!official||!keys(*official,{"repository","commit","license","mode","scope"})||!text(*official,"repository",result.official_repository)||
           !text(*official,"commit",result.official_commit,40)||result.official_commit.size()!=40u||!text(*official,"mode",result.official_mode)||
           !text(*official,"license",ignored)||!text(*official,"scope",ignored)||!ros||!keys(*ros,{"release","os_abi","rmw","transport","security"})) { return error::malformed; }
        for(auto c:result.official_commit) { if(!((c>='0'&&c<='9')||(c>='a'&&c<='f'))) { return error::malformed; } }
        for(auto key:{"release","os_abi","rmw","transport","security"}) { if(!text(*ros,key,ignored)) { return error::malformed; } }
        auto resources{adapter.at("resources")};auto granted{policy.at("resources")};
        if(!resources||!granted) { return error::malformed; }
        if(explicit_controller)
        {
            if(!keys(*resources,{"joint_state_topic","position_action","controller_status_service","controller_type"})||
               !keys(*granted,{"joint_state_topic","position_action","controller_status_service","controller_type"})) { return error::malformed; }
            if(!text(*resources,"controller_type",result.controller_type,128)||!equal(*granted,"controller_type",result.controller_type)) { return error::denied; }
            // Opt-in deployment profile, with the exact native class granted
            // separately. Both use the checked full FollowJointTrajectory
            // result/UUID/state path; arbitrary controller classes are refused.
            if(result.controller_type!="joint_trajectory_controller/JointTrajectoryController"&&
               result.controller_type!="fanuc_controllers/ScaledJointTrajectoryController") { return error::unsupported; }
        }
        else if(!keys(*resources,{"joint_state_topic","position_action","controller_status_service"})||
                !keys(*granted,{"joint_state_topic","position_action","controller_status_service"})) { return error::malformed; }
        for(auto field:{::std::pair{"joint_state_topic",&result.state_topic},::std::pair{"position_action",&result.position_action},::std::pair{"controller_status_service",&result.controller_service}})
        { if(!text(*resources,field.first,*field.second)||!name(*field.second,true)||!equal(*granted,field.first,*field.second)) { return error::denied; } }
        auto joints{adapter.at("joints")};if(!joints||joints->type!=value::kind::array||joints->elements.empty()||joints->elements.size()>max_joints) { return error::malformed; }
        result.mapping.count=static_cast<::std::uint32_t>(joints->elements.size());::std::array<bool,max_joints> used{};
        for(auto const& j:joints->elements)
        {
            ::std::uint64_t slot{},sign{};::std::int64_t offset{};
            if(!keys(j,{"slot","native_joint","kind","source_unit","sign","zero_offset_microrad","lower_microrad","upper_microrad","max_step_microrad","tolerance_microrad"})||
               !number(j,"slot",slot,result.mapping.count-1u)||used[slot]||!equal(j,"kind","bounded_revolute")||!equal(j,"source_unit","rad")) { return error::malformed; }
            // The preview does not define sign/offset composition order. Refuse
            // nonidentity transforms until a versioned rule can be verified.
            if(!number(j,"sign",sign,1)||sign!=1||!position(j,"zero_offset_microrad",offset)||offset) { return error::unsupported; }
            used[slot]=true;auto& limits{result.mapping.joints[slot]};
            // The v0.1 joint field is a bounded exact UTF-8 message string,
            // not a ROS graph identifier. Preserve spaces and Unicode exactly;
            // parser validation, uniqueness and the complete adapter pin apply.
            if(!text(j,"native_joint",result.native_joints[slot],128)||
               !position(j,"lower_microrad",limits.lower)||!position(j,"upper_microrad",limits.upper)||
               !number(j,"max_step_microrad",limits.max_step,10000)||!number(j,"tolerance_microrad",limits.tolerance,1000)) { return error::malformed; }
            for(::std::size_t i{};i!=max_joints;++i) { if(i!=slot&&used[i]&&result.native_joints[i]==result.native_joints[slot]) { return error::malformed; } }
        }
        if(!valid(result.mapping)) { return error::malformed; }output=::std::move(result);return error::ok;
    }

    class deployment_selection final
    {
        inline static ::std::shared_ptr<joint_deployment const> selected_{};
    public:
        using owner=::std::shared_ptr<joint_deployment const>;
        [[nodiscard]] static owner current() noexcept { return selected_; }
        // Trusted command-line setup only, before any Wasm execution. The
        // separate CLI adapter pin binds exact adapter bytes without making
        // policy and adapter files contain each other's cryptographic hashes.
        static error configure(::std::string const& adapter_path,::std::string_view adapter_hash,
                               ::std::string const& policy_path,::std::string_view policy_hash)
        {
            if(selected_) { return error::denied; }digest a{},p{};
            if(!parse_digest(adapter_hash,a)||!parse_digest(policy_hash,p)) { return error::malformed; }
            ::std::string adapter{},policy{};
            if(auto e{deployment_detail::read_exact_file(adapter_path,a,adapter)};e!=error::ok) { return e; }
            if(auto e{deployment_detail::read_exact_file(policy_path,p,policy)};e!=error::ok) { return e; }
            joint_deployment result{};if(auto e{parse_joint_deployment(adapter,policy,result)};e!=error::ok) { return e; }
            selected_=::std::make_shared<joint_deployment const>(::std::move(result));return error::ok;
        }
    };
}
