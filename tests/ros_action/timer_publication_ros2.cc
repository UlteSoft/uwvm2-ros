#include "../../gateways/ros2/action.h"
#include <atomic>
#include <condition_variable>
#include <iostream>
#include <stdexcept>
#include <thread>

namespace action_timer_test
{
    namespace action=uwvm2::ros_gateway::ros2_action;
    using timer_ptr=rclcpp::TimerBase::SharedPtr;
    using goal_ptr=std::shared_ptr<action::goal_state>;

    void require(bool condition,char const* message)
    { if(!condition) { throw std::runtime_error(message); } }

    struct destruction_observation
    {
        std::atomic<unsigned> destroyed{},locked{},missing{};
    };
    struct destruction_probe
    {
        std::weak_ptr<action::goal_state> goal;
        std::shared_ptr<destruction_observation> observation;
        destruction_probe(goal_ptr const& state,std::shared_ptr<destruction_observation> observed):goal{state},observation{std::move(observed)} {}
        ~destruction_probe()
        {
            auto state{goal.lock()};
            if(!state) { ++observation->missing;return; }
            // Probe from another thread: try_lock by the owning thread would
            // itself be undefined for std::mutex. These destruction cases have
            // no competing publisher, so a failure identifies a held goal lock.
            try
            {
                std::thread inspect{[&] {
                    if(state->mutex.try_lock()) { state->mutex.unlock(); }
                    else { ++observation->locked; }
                }};
                inspect.join();
            }
            catch(...) { ++observation->locked; }
            ++observation->destroyed;
        }
    };
    timer_ptr make_timer(rclcpp::Node::SharedPtr const& node)
    { return node->create_wall_timer(std::chrono::hours(1),[] {}); }
    timer_ptr watched_timer(rclcpp::Node::SharedPtr const& node,goal_ptr const& goal,std::shared_ptr<destruction_observation> const& observation)
    {
        auto probe{std::make_shared<destruction_probe>(goal,observation)};
        return node->create_wall_timer(std::chrono::hours(1),[probe] {});
    }
    bool retained(goal_ptr const& goal)
    { std::lock_guard<std::mutex> lock{goal->mutex};return static_cast<bool>(goal->timer); }
    void require_destroyed(destruction_observation const& observation,unsigned expected)
    {
        require(observation.destroyed==expected,"actual Timer callback ownership was not released");
        require(!observation.locked&&!observation.missing,"Timer destruction occurred with goal mutex held or missing goal");
    }

    void terminal_before_publication(rclcpp::Node::SharedPtr const& node)
    {
        auto goal{std::make_shared<action::goal_state>()};auto observation{std::make_shared<destruction_observation>()};
        auto timer{watched_timer(node,goal,observation)};
        { std::lock_guard<std::mutex> lock{goal->mutex};goal->terminal=true; }
        require(!action::publish_deadline_timer(goal,timer),"terminal goal accepted a timer");
        require(!retained(goal)&&timer->is_canceled(),"terminal publication retained or failed to cancel timer");
        timer.reset();require_destroyed(*observation,1);
    }
    void stop_before_publication(rclcpp::Node::SharedPtr const& node)
    {
        auto goal{std::make_shared<action::goal_state>()};auto observation{std::make_shared<destruction_observation>()};
        auto timer{watched_timer(node,goal,observation)};action::stop_goal(goal);
        require(!action::publish_deadline_timer(goal,timer),"stopped goal accepted a timer");
        require(!retained(goal)&&timer->is_canceled(),"stop-before-publish retained or failed to cancel timer");
        timer.reset();require_destroyed(*observation,1);
    }
    void replace_and_retire(rclcpp::Node::SharedPtr const& node)
    {
        auto goal{std::make_shared<action::goal_state>()};auto observation{std::make_shared<destruction_observation>()};
        auto first{watched_timer(node,goal,observation)},second{watched_timer(node,goal,observation)};
        require(action::publish_deadline_timer(goal,first),"active goal did not retain first timer");
        require(action::publish_deadline_timer(goal,second),"active goal did not retain replacement timer");
        require(first->is_canceled()&&!second->is_canceled(),"replacement canceled the wrong timer");
        first.reset();require_destroyed(*observation,1);
        require(action::publish_deadline_timer(goal,second)&&!second->is_canceled(),"publishing the same timer canceled it");
        std::weak_ptr<rclcpp::TimerBase> weak{second};second.reset();
        action::stop_goal(goal);
        require(!retained(goal)&&weak.expired(),"stop retained the published timer");
        require_destroyed(*observation,2);
    }

    class phase_gate
    {
        std::mutex mutex;
        std::condition_variable condition;
        unsigned phase{};
    public:
        void advance(unsigned value)
        { { std::lock_guard<std::mutex> lock{mutex};phase=std::max(phase,value); }condition.notify_all(); }
        bool wait(unsigned value)
        { std::unique_lock<std::mutex> lock{mutex};return condition.wait_for(lock,std::chrono::seconds(3),[&] { return phase>=value; }); }
    };
    void barrier_retirement(rclcpp::Node::SharedPtr const& node,bool terminal)
    {
        auto goal{std::make_shared<action::goal_state>()};phase_gate gate;
        std::atomic<bool> published{true},canceled{},failed{};
        std::thread publisher{[&] {
            try
            {
                auto timer{make_timer(node)};gate.advance(1);
                if(!gate.wait(2)) { failed=true;return; }
                published=action::publish_deadline_timer(goal,timer);canceled=timer->is_canceled();
            }
            catch(...) { failed=true;gate.advance(1); }
        }};
        bool ready{gate.wait(1)};
        if(terminal) { std::lock_guard<std::mutex> lock{goal->mutex};goal->terminal=true; }
        else { action::stop_goal(goal); }
        gate.advance(2);publisher.join();
        require(ready&&!failed,"timer creation/publication barrier failed");
        require(!published&&canceled&&!retained(goal),"retirement between creation and publication retained a timer");
    }
    void concurrent_replace_stop(rclcpp::Node::SharedPtr const& node)
    {
        // Both legal serialization orders must end with no retained timer.
        // Keep native timer references to independently inspect cancellation.
        for(unsigned round{};round!=64;++round)
        {
            auto goal{std::make_shared<action::goal_state>()};auto first{make_timer(node)},second{make_timer(node)};
            require(action::publish_deadline_timer(goal,first),"race setup could not publish first timer");
            phase_gate gate;std::atomic<bool> failed{};
            std::thread publisher{[&] {
                gate.advance(1);if(!gate.wait(2)) { failed=true;return; }
                try { action::publish_deadline_timer(goal,second); }catch(...) { failed=true; }
            }};
            bool ready{gate.wait(1)};gate.advance(2);action::stop_goal(goal);publisher.join();
            require(ready&&!failed,"concurrent replacement barrier failed");
            require(!retained(goal)&&first->is_canceled()&&second->is_canceled(),"replace/stop race left a live native timer");
        }
    }
}

int main(int argc,char** argv)
{
    try
    {
        rclcpp::init(argc,argv);
        auto options{rclcpp::NodeOptions{}.start_parameter_services(false).start_parameter_event_publisher(false).enable_rosout(false)};
        options.parameter_overrides({rclcpp::Parameter("start_type_description_service",false)});
        auto node{std::make_shared<rclcpp::Node>("action_timer_publication",options)};
        action_timer_test::terminal_before_publication(node);
        action_timer_test::stop_before_publication(node);
        action_timer_test::replace_and_retire(node);
        action_timer_test::barrier_retirement(node,true);
        action_timer_test::barrier_retirement(node,false);
        action_timer_test::concurrent_replace_stop(node);
        node.reset();rclcpp::shutdown();
        std::cout<<"PASS actual ROS2 Timer terminal/stop-before-publication, replacement/same-pointer, two deterministic retirement barriers, 64 replace/stop races and four lock-free callback destructions\n";
        return 0;
    }
    catch(std::exception const& error)
    { if(rclcpp::ok()) { rclcpp::shutdown(); }std::cerr<<"FAIL "<<error.what()<<'\n';return 1; }
}
