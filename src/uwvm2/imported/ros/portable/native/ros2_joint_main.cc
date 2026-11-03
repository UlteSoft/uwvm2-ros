// SPDX-License-Identifier: Apache-2.0
// Linux ROS 2 native process for one explicitly granted software-simulation step.
// This is never a Guest import provider and never creates a ROS action server.
#include <uwvm2/imported/ros/portable/native_client.h>
#include <uwvm2/imported/ros/portable/runtime_audit.h>
#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>
#include <control_msgs/action/follow_joint_trajectory.hpp>
#include <controller_manager_msgs/srv/list_controllers.hpp>
#include <sensor_msgs/msg/joint_state.hpp>
#include <trajectory_msgs/msg/joint_trajectory_point.hpp>
#include <rmw/rmw.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <future>
#include <optional>
#include <set>
#include <sstream>
#include <stdexcept>
#if !defined(__linux__) || !defined(__cpp_exceptions)
# error The native joint helper requires Linux and checked C++ exception unwinding.
#endif

namespace p=uwvm2::imported::ros::portable;
namespace ipc=uwvm2::imported::ros::ipc;
namespace config=p::deployment_detail;
using Action=control_msgs::action::FollowJointTrajectory;
using Goal=rclcpp_action::ClientGoalHandle<Action>;
using Controllers=controller_manager_msgs::srv::ListControllers;
using clock_type=std::chrono::steady_clock;
using ns=std::chrono::nanoseconds;

static void need(bool yes,char const* message) { if(!yes) { throw std::runtime_error(message); } }
static clock_type::time_point until(std::uint64_t value)
{ need(value<=static_cast<std::uint64_t>(INT64_MAX),"deadline overflow");return clock_type::time_point{ns{static_cast<std::int64_t>(value)}}; }
struct fd_owner
{
    int value{-1};
    explicit fd_owner(int fd=-1):value{fd}{}
    ~fd_owner() { if(value>=0) { ::close(value); } }
    fd_owner(fd_owner const&)=delete;fd_owner& operator=(fd_owner const&)=delete;
};
static bool same_file(struct stat const& a,struct stat const& b)
{
    return a.st_dev==b.st_dev&&a.st_ino==b.st_ino&&a.st_size==b.st_size&&
        a.st_mtim.tv_sec==b.st_mtim.tv_sec&&a.st_mtim.tv_nsec==b.st_mtim.tv_nsec&&
        a.st_ctim.tv_sec==b.st_ctim.tv_sec&&a.st_ctim.tv_nsec==b.st_ctim.tv_nsec;
}
struct image_pin
{
    std::string path;fd_owner fd;struct stat identity{};
    image_pin(std::string name,p::digest const& expected,clock_type::time_point deadline):path{std::move(name)},fd{::open(path.c_str(),O_RDONLY|O_CLOEXEC)}
    {
        need(fd.value>=0&&::fstat(fd.value,&identity)==0&&S_ISREG(identity.st_mode)&&identity.st_size>0&&identity.st_size<=512*1024*1024,"native image is not a bounded regular file");
        fast_io::sha256_context hash{};std::array<std::byte,65536> bytes{};off_t offset{};
        while(offset!=identity.st_size)
        {
            need(clock_type::now()<deadline,"native image verification deadline");
            auto count=::pread(fd.value,bytes.data(),std::min<std::size_t>(bytes.size(),static_cast<std::size_t>(identity.st_size-offset)),offset);
            if(count<0&&errno==EINTR) { continue; }need(count>0,"native image short read");hash.update(bytes.data(),bytes.data()+count);offset+=count;
        }
        p::digest actual{};hash.do_final();hash.digest_to_byte_ptr(reinterpret_cast<std::byte*>(actual.data()));char extra{};
        need(actual==expected&&::pread(fd.value,&extra,1,identity.st_size)==0&&unchanged(),"native image pin mismatch");
    }
    bool unchanged() const
    { struct stat opened{},selected{};return ::fstat(fd.value,&opened)==0&&::stat(path.c_str(),&selected)==0&&same_file(identity,opened)&&same_file(identity,selected); }
};
static std::set<std::string> executable_images()
{
    std::ifstream input{"/proc/self/maps"};need(input.good(),"native executable mappings unavailable");
    std::set<std::string> paths;std::string line;unsigned count{};
    while(std::getline(input,line))
    {
        need(++count<=4096&&line.size()<=8192,"native mapping census exceeded");
        std::istringstream row{line};std::string extent,permissions,offset,device,inode,path;
        need(static_cast<bool>(row>>extent>>permissions>>offset>>device>>inode),"malformed native mapping");
        if(permissions.size()!=4||permissions[2]!='x') { continue; }
        std::getline(row,path);auto start=path.find_first_not_of(' ');need(start!=path.npos,"anonymous executable mapping refused");path.erase(0,start);
        if(path=="[vdso]"||path=="[vsyscall]") { continue; }
        need(path.front()=='/'&&path.find(" (deleted)")==path.npos&&path.find('\\')==path.npos,"unidentified native executable mapping");paths.insert(path);
    }
    need(!paths.empty()&&paths.size()<=128,"native executable image count invalid");return paths;
}
class provenance final
{
    std::set<std::string> paths_;
    std::vector<std::unique_ptr<image_pin>> pins_;
public:
    provenance(p::joint_deployment const& d,std::string const& file)
    {
        std::string bytes;need(config::read_exact_file(file,d.native_provenance,bytes)==p::error::ok,"native provenance file pin refused");
        config::value v;need(config::parser{bytes}.parse(v)&&config::keys(v,{"format","execution_class","official_repository","official_commit","official_mode","rmw","images"}),"native provenance schema refused");
        auto actual_rmw=rmw_get_implementation_identifier();need(actual_rmw!=nullptr,"native RMW identity unavailable");
        need(config::equal(v,"format","uwvm-portable-native-provenance-v1")&&config::equal(v,"execution_class","software_simulation")&&
             config::equal(v,"official_repository",d.official_repository)&&config::equal(v,"official_commit",d.official_commit)&&
             config::equal(v,"official_mode",d.official_mode)&&config::equal(v,"rmw",actual_rmw),"native provenance deployment mismatch");
        auto images=v.at("images");need(images&&images->type==config::value::kind::array&&!images->elements.empty()&&images->elements.size()<=128,"native provenance image set absent");
        auto deadline=clock_type::now()+std::chrono::seconds{15};
        for(auto const& image:images->elements)
        {
            std::string path;p::digest hash{};need(config::keys(image,{"path","sha256"})&&config::text(image,"path",path,4096)&&path.front()=='/'&&config::hex(image,"sha256",hash),"native image entry refused");
            need(paths_.insert(path).second,"duplicate native image entry");pins_.push_back(std::make_unique<image_pin>(path,hash,deadline));
        }
        need(unchanged(),"loaded native images differ from exact provenance");
    }
    bool unchanged() const
    { if(executable_images()!=paths_) { return false; }for(auto const& pin:pins_) { if(!pin->unchanged()) { return false; } }return true; }
};

class joint_backend final
{
    p::joint_deployment const& d_;
    rclcpp::Node::SharedPtr node_;
    rclcpp::executors::SingleThreadedExecutor executor_;
    rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr states_;
    rclcpp::Client<Controllers>::SharedPtr controllers_;
    rclcpp_action::Client<Action>::SharedPtr action_;
    Goal::SharedPtr goal_;
    std::optional<Goal::WrappedResult> terminal_;
    p::state latest_{},offered_{};p::command command_{};
    std::uint64_t source_stamp_{},accepted_ns_{},terminal_ns_{},command_deadline_{};
    bool consumed_{},goal_response_{},drain_started_{},drain_complete_{};
    std::string controller_name_;

    template<typename Predicate> bool spin_until(Predicate ready,std::uint64_t deadline)
    {
        while(rclcpp::ok()&&p::monotonic_ns()<deadline)
        { executor_.spin_some(std::chrono::milliseconds{2});if(ready()) { return true; }::poll(nullptr,0,1); }
        return false;
    }
    void observe(sensor_msgs::msg::JointState const& message)
    {
        if(message.name.empty()||message.name.size()>256||message.name.size()!=message.position.size()||
           message.header.stamp.sec<0||message.header.stamp.nanosec>=1000000000u) { return; }
        auto stamp=static_cast<std::uint64_t>(message.header.stamp.sec)*1000000000u+message.header.stamp.nanosec;
        if(stamp<=source_stamp_||latest_.sequence==UINT64_MAX) { return; }
        std::set<std::string> names;for(auto const& name:message.name) { if(!names.insert(name).second) { return; } }
        p::state next{};next.epoch=d_.mapping.epoch;next.count=d_.mapping.count;next.sequence=latest_.sequence+1;
        for(std::uint32_t slot{};slot!=next.count;++slot)
        {
            auto found=std::find(message.name.begin(),message.name.end(),d_.native_joints[slot]);if(found==message.name.end()) { return; }
            double radians=message.position[static_cast<std::size_t>(found-message.name.begin())];
            if(!std::isfinite(radians)||radians < -1000.0||radians > 1000.0) { return; }
            next.positions[slot]=std::llround(radians*1000000.0);
            auto const& bound=d_.mapping.joints[slot];if(next.positions[slot]<bound.lower||next.positions[slot]>bound.upper) { return; }
        }
        next.observed_ns=p::monotonic_ns();next.valid_until_ns=next.observed_ns+250000000u;
        source_stamp_=stamp;latest_=next;
    }
    bool controller(std::uint64_t deadline)
    {
        if(!spin_until([&]{return controllers_->service_is_ready()&&action_->action_server_is_ready();},deadline)) { return false; }
        auto future=controllers_->async_send_request(std::make_shared<Controllers::Request>());
        if(!spin_until([&]{return future.wait_for(ns{0})==std::future_status::ready;},deadline))
        { controllers_->remove_pending_request(future);return false; }
        auto answer=future.get();if(!answer||answer->controller.size()>256) { return false; }
        unsigned matches{};std::set<std::string> expected;
        for(std::uint32_t i{};i!=d_.mapping.count;++i) { expected.insert(d_.native_joints[i]+"/position"); }
        for(auto const& entry:answer->controller)
        {
            if(entry.name!=controller_name_) { continue; }++matches;
            if(entry.state!="active"||entry.type!=d_.controller_type||entry.claimed_interfaces.size()!=expected.size()||
               std::set<std::string>(entry.claimed_interfaces.begin(),entry.claimed_interfaces.end())!=expected) { return false; }
        }
        return matches==1;
    }
    static std::vector<std::uint8_t> encode_event(Goal::SharedPtr const& goal,std::uint64_t observed,p::outcome outcome,std::int32_t status)
    {
        std::vector<std::uint8_t> bytes(32);auto const& uuid=goal->get_goal_id();std::copy(uuid.begin(),uuid.end(),bytes.begin());
        p::wire_detail::put(bytes,16,observed,8);p::wire_detail::put(bytes,24,static_cast<unsigned>(outcome),4);
        p::wire_detail::put(bytes,28,std::bit_cast<std::uint32_t>(status),4);return bytes;
    }
public:
    explicit joint_backend(p::joint_deployment const& deployment):d_{deployment}
    {
        auto action=d_.position_action;std::string const suffix="/follow_joint_trajectory";
        need(action.ends_with(suffix),"only the explicit JTC action profile is implemented");action.resize(action.size()-suffix.size());
        auto slash=action.find_last_of('/');need(slash!=action.npos&&slash+1<action.size(),"controller action name invalid");controller_name_=action.substr(slash+1);
        need(d_.controller_service==action.substr(0,slash)+"/controller_manager/list_controllers","controller manager namespace differs from action namespace");
        auto options=rclcpp::NodeOptions{}.use_global_arguments(false).enable_rosout(false).start_parameter_services(false).start_parameter_event_publisher(false);
        node_=std::make_shared<rclcpp::Node>("uwvm_portable_joint_"+std::to_string(d_.native_generation),options);executor_.add_node(node_);
        states_=node_->create_subscription<sensor_msgs::msg::JointState>(d_.state_topic,rclcpp::SensorDataQoS{},[this](sensor_msgs::msg::JointState::ConstSharedPtr m){if(m) { observe(*m); }});
        controllers_=node_->create_client<Controllers>(d_.controller_service);action_=rclcpp_action::create_client<Action>(node_,d_.position_action);
    }
    p::error read(std::uint64_t deadline,std::vector<std::uint8_t>& output)
    {
        if(!controller(deadline)||!spin_until([&]{return latest_.sequence&&p::monotonic_ns()<latest_.valid_until_ns;},deadline)) { return p::error::no_fresh_state; }
        offered_=latest_;output.resize(64u+16u*offered_.count);return p::encode(offered_,output).status;
    }
    p::error submit(std::span<std::uint8_t const> bytes,std::uint64_t deadline,std::vector<std::uint8_t>& output)
    {
        if(consumed_) { return p::error::bad_handle; }p::command command{};
        if(!p::decode(bytes,command)) { return p::error::malformed; }
        if(auto e=p::validate_command_data(command,d_.mapping,offered_,p::monotonic_ns());e!=p::error::ok) { return e; }
        if(!controller(deadline)) { return p::error::denied; }
        // Recheck the offered sample after discovery; discovery consumes the
        // same deadline and cannot revive an old observation.
        if(auto e=p::validate_command_data(command,d_.mapping,offered_,p::monotonic_ns());e!=p::error::ok) { return e; }
        for(std::uint32_t i{};i!=command.count;++i)
        { auto distance=std::llabs(latest_.positions[i]-offered_.positions[i]);if(static_cast<std::uint64_t>(distance)>d_.mapping.joints[i].tolerance) { return p::error::no_fresh_state; } }
        Action::Goal goal{};trajectory_msgs::msg::JointTrajectoryPoint point{};
        for(std::uint32_t i{};i!=command.count;++i)
        { goal.trajectory.joint_names.push_back(d_.native_joints[i]);point.positions.push_back(static_cast<double>(command.targets[i])/1000000.0); }
        point.time_from_start.sec=static_cast<std::int32_t>(command.duration_ns/1000000000u);point.time_from_start.nanosec=static_cast<std::uint32_t>(command.duration_ns%1000000000u);
        goal.trajectory.points.push_back(std::move(point));
        rclcpp_action::Client<Action>::SendGoalOptions options{};
        options.goal_response_callback=[this](Goal::SharedPtr handle){goal_response_=true;goal_=std::move(handle);accepted_ns_=p::monotonic_ns();};
        options.result_callback=[this](Goal::WrappedResult const& result){terminal_=result;terminal_ns_=p::monotonic_ns();};
        // Never reset this state after an exception or ambiguous send.
        consumed_=true;command_=command;command_deadline_=deadline;(void)action_->async_send_goal(goal,options);
        if(!spin_until([&]{return goal_response_;},deadline)) { return p::error::deadline; }
        if(!goal_) { return p::error::denied; }
        output=encode_event(goal_,accepted_ns_,p::outcome::accepted,0);return p::error::ok;
    }
    p::error wait(std::span<std::uint8_t const> request,std::uint64_t deadline,std::vector<std::uint8_t>& output)
    {
        if(!goal_||request.size()!=24||deadline!=command_deadline_||p::wire_detail::get(request,16,8)!=command_.id||
           !std::equal(goal_->get_goal_id().begin(),goal_->get_goal_id().end(),request.begin())) { return p::error::bad_handle; }
        if(!spin_until([&]{return terminal_.has_value();},deadline)) { return p::error::deadline; }
        auto const& native=*terminal_;if(native.goal_id!=goal_->get_goal_id()||!native.result||terminal_ns_<accepted_ns_) { return p::error::malformed; }
        p::outcome outcome{};
        if(native.code==rclcpp_action::ResultCode::SUCCEEDED&&native.result->error_code==Action::Result::SUCCESSFUL) { outcome=p::outcome::succeeded; }
        else if(native.code==rclcpp_action::ResultCode::CANCELED) { outcome=p::outcome::cancelled; }
        else if(native.code==rclcpp_action::ResultCode::ABORTED||native.code==rclcpp_action::ResultCode::SUCCEEDED) { outcome=p::outcome::rejected; }
        else { return p::error::malformed; }
        output=encode_event(goal_,terminal_ns_,outcome,native.result->error_code);
        if(outcome==p::outcome::succeeded)
        {
            if(!controller(deadline)||!spin_until([&]{
                if(latest_.sequence<=command_.based_on_sequence||latest_.observed_ns<terminal_ns_||p::monotonic_ns()>=latest_.valid_until_ns) { return false; }
                for(std::uint32_t i{};i!=command_.count;++i)
                { auto distance=std::llabs(latest_.positions[i]-command_.targets[i]);if(static_cast<std::uint64_t>(distance)>d_.mapping.joints[i].tolerance) { return false; } }return true;
            },deadline)) { return p::error::no_fresh_state; }
            auto start=output.size();output.resize(start+64u+16u*latest_.count);
            if(p::encode(latest_,std::span{output}.subspan(start)).status!=p::error::ok) { return p::error::malformed; }
        }
        return p::error::ok;
    }
    bool drain_owned() noexcept
    {
        if(drain_started_) { return drain_complete_; }drain_started_=true;
        try
        {
            if(!consumed_||terminal_||(goal_response_&&!goal_)) { drain_complete_=true;return true; }
            auto deadline=p::monotonic_ns()+p::command_state::maximum_wait_ns;
            if(!goal_response_&&!spin_until([&]{return goal_response_;},deadline)) { return false; }
            if(!goal_||terminal_) { drain_complete_=true;return true; }
            // Only this accepted UUID. Never cancel-all or retry a goal.
            (void)action_->async_cancel_goal(goal_);
            drain_complete_=spin_until([&]{return terminal_&&terminal_->goal_id==goal_->get_goal_id();},deadline);return drain_complete_;
        }
        catch(...) { return false; }
    }
};

static bool valid_binding(std::span<std::uint8_t const> bytes,p::joint_deployment const& d)
{
    if(bytes.size()<168) { return false; }std::size_t at{};
    for(auto const* hash:{&d.wasm_sha256,&d.adapter_sha256,&d.policy_sha256,&d.native_provenance})
    { if(!std::equal(hash->begin(),hash->end(),bytes.begin()+static_cast<std::ptrdiff_t>(at))) { return false; }at+=32; }
    return p::wire_detail::get(bytes,128,8)>0&&p::wire_detail::get(bytes,136,8)==d.policy_epoch&&
           p::wire_detail::get(bytes,144,8)==d.mapping.epoch&&p::wire_detail::get(bytes,152,8)==d.native_generation&&p::wire_detail::get(bytes,160,8)>0;
}
static void serve(int fd,p::joint_deployment const& d,joint_backend& backend,provenance const& origin,image_pin const& peer)
{
    std::array<std::uint8_t,168> binding{};std::uint64_t session{},next{1};bool bound{};p::digest command_sha256{};
    for(unsigned requests{};requests!=32;++requests)
    {
        std::array<std::uint8_t,40> header{};
        if(!ipc::transfer(fd,header.data(),header.size(),false,clock_type::now()+std::chrono::seconds{5})) { return; }
        auto size=p::wire_detail::get(header,8,4),deadline=p::wire_detail::get(header,32,8),now=p::monotonic_ns();
        auto operation=p::wire_detail::get(header,6,2),incoming_session=p::wire_detail::get(header,24,8);
        need(header[0]=='U'&&header[1]=='P'&&header[2]=='N'&&header[3]=='1'&&p::wire_detail::get(header,4,2)==1&&
             p::wire_detail::get(header,12,4)==0&&p::wire_detail::get(header,16,8)==next&&size>=168&&size<=4264&&
             incoming_session&&(bound?incoming_session==session:operation==1)&&deadline>now&&deadline-now<=p::command_state::maximum_wait_ns,"native request header refused");
        std::vector<std::uint8_t> input(static_cast<std::size_t>(size));need(ipc::transfer(fd,input.data(),input.size(),false,until(deadline)),"native request body incomplete");
        need(peer.unchanged()&&origin.unchanged()&&valid_binding(input,d),"native peer/provenance/binding changed");
        if(bound) { need(std::equal(binding.begin(),binding.end(),input.begin()),"owner binding changed"); }
        else { std::copy_n(input.begin(),168,binding.begin());session=incoming_session; }
        auto body=std::span<std::uint8_t const>{input}.subspan(168);std::vector<std::uint8_t> output;p::error status=p::error::malformed;
        switch(operation)
        {
            case 1:if(!bound&&body.empty()) { bound=true;status=p::error::ok; }break;
            case 2:if(bound&&body.empty()) { status=backend.read(deadline,output); }break;
            case 3:if(bound) { status=backend.submit(body,deadline,output); }break;
            case 4:if(bound) { status=backend.wait(body,deadline,output); }break;
            case 5:if(bound&&body.empty()) { status=backend.drain_owned()?p::error::ok:p::error::deadline; }break;
            default:break;
        }
        if(status!=p::error::ok) { output.clear(); }need(output.size()<=4096,"native reply bound exceeded");
        p::wire_detail::put(header,8,168+output.size(),4);p::wire_detail::put(header,12,static_cast<unsigned>(status),4);
        auto const reply_prefix=header.size()+binding.size();
        std::vector<std::uint8_t> reply(reply_prefix+output.size());
        std::copy(header.begin(),header.end(),reply.begin());
        std::copy(binding.begin(),binding.end(),reply.begin()+header.size());
        std::copy(output.begin(),output.end(),reply.begin()+reply_prefix);
        need(ipc::transfer(fd,reply.data(),reply.size(),true,until(deadline)),"native reply deadline");++next;
        if(p::runtime_audit::runtime.enabled()&&status==p::error::ok)
        {
            if(operation==3) { command_sha256=p::hash_bytes({reinterpret_cast<char const*>(body.data()),body.size()}); }
            else if(operation==4&&output.size()>=32u)
            {
                p::native_event event{};p::state confirmed{};
                if(p::decode_native_event({output.data(),32},event)&&
                   (event.result!=p::outcome::succeeded||p::decode({output.data()+32,output.size()-32},confirmed)))
                {
                    (void)p::runtime_audit::runtime.emit(d,
                        {p::runtime_audit::event::native_terminal,p::wire_detail::get(binding,128,8),p::wire_detail::get(binding,160,8),
                         p::wire_detail::get(body,16,8),event.observed_ns,confirmed.sequence,event.goal_uuid,event.result,event.status,command_sha256});
                }
            }
        }
        if(status!=p::error::ok||operation==5) { return; }
    }
    throw std::runtime_error("finite native request count exhausted");
}
int main(int argc,char** argv)
{
    if(argc!=7&&argc!=9) { std::fprintf(stderr,"usage: native-joint adapter-path adapter-sha256 grant-path grant-sha256 provenance-path vm-executable-sha256 [--portable-robot-audit-run run-id]\n");return 2; }
    if(argc==9&&(std::string_view{argv[7]}!="--portable-robot-audit-run"||!p::runtime_audit::runtime.configure(argv[8])))
    { std::fprintf(stderr,"native-joint: invalid explicit portable audit run ID\n");return 2; }
    std::unique_ptr<joint_backend> backend;std::string owned_socket;struct stat socket_identity{};int result=1;
    try
    {
        need(p::deployment_selection::configure(argv[1],argv[2],argv[3],argv[4])==p::error::ok,"deployment refused");auto d=p::deployment_selection::current();
        need(d->native_uid==::getuid(),"native process uid differs from grant");p::digest vm_hash{};need(p::parse_digest(argv[6],vm_hash),"VM executable pin invalid");
        image_pin self{"/proc/self/exe",d->native_executable,clock_type::now()+std::chrono::seconds{2}};
        rclcpp::init(0,nullptr);backend=std::make_unique<joint_backend>(*d);
        std::vector<std::uint8_t> initial_state;
        need(backend->read(p::monotonic_ns()+5000000000u,initial_state)==p::error::ok,"native controller and fresh-state readiness deadline");
        provenance origin{*d,argv[5]};
        auto slash=d->socket.find_last_of('/');need(slash!=d->socket.npos&&slash>0,"private socket directory required");
        struct stat directory{};auto parent=d->socket.substr(0,slash);
        need(::lstat(parent.c_str(),&directory)==0&&S_ISDIR(directory.st_mode)&&directory.st_uid==::getuid()&&(directory.st_mode&0077)==0,"socket directory must be private and caller-owned");
        fd_owner listener{::socket(AF_UNIX,SOCK_STREAM|SOCK_CLOEXEC|SOCK_NONBLOCK,0)};need(listener.value>=0,"native socket creation failed");
        sockaddr_un address{};address.sun_family=AF_UNIX;std::memcpy(address.sun_path,d->socket.c_str(),d->socket.size()+1);
        auto old_umask=::umask(0077);auto bound=::bind(listener.value,reinterpret_cast<sockaddr*>(&address),sizeof address);::umask(old_umask);
        need(bound==0,"native socket path already exists or bind failed");owned_socket=d->socket;
        need(::lstat(owned_socket.c_str(),&socket_identity)==0&&S_ISSOCK(socket_identity.st_mode)&&::listen(listener.value,1)==0,"native listener failed");
        pollfd pending{listener.value,POLLIN,0};need(::poll(&pending,1,30000)>0,"finite VM connect deadline");
        fd_owner client{::accept4(listener.value,nullptr,nullptr,SOCK_CLOEXEC|SOCK_NONBLOCK)};need(client.value>=0,"native accept failed");
        std::uint32_t uid{},pid{};need(ipc::peer_credentials(client.value,uid,pid)&&uid==d->native_uid&&pid!=static_cast<std::uint32_t>(::getpid()),"VM peer credentials refused");
        image_pin peer{"/proc/"+std::to_string(pid)+"/exe",vm_hash,clock_type::now()+std::chrono::seconds{2}};
        serve(client.value,*d,*backend,origin,peer);result=backend->drain_owned()?0:3;
    }
    catch(std::exception const& error) { std::fprintf(stderr,"native-joint: %s\n",error.what()); }
    catch(...) { std::fprintf(stderr,"native-joint: non-standard checked exception\n"); }
    if(backend&&!backend->drain_owned()) { result=3; }
    backend.reset();if(rclcpp::ok()) { rclcpp::shutdown(); }
    if(!owned_socket.empty())
    { struct stat current{};if(::lstat(owned_socket.c_str(),&current)==0&&current.st_dev==socket_identity.st_dev&&current.st_ino==socket_identity.st_ino) { (void)::unlink(owned_socket.c_str()); } }
    return result;
}
