package org.uwvm.rcljava;
import org.ros2.rcljava.RCLJava;
import org.ros2.rcljava.migration.Deployment;
import org.ros2.rcljava.node.ComposableNode;
import org.ros2.rcljava.executors.SingleThreadedExecutor;
import org.uwvm.roswasm.Result;
import org.uwvm.roswasm.Codecs;
/** This source fixture compiles/constructs actual unchanged official examples
 * against the opt-in API. The explicit deterministic Model supplies no native
 * ROS/RMW/JNI/Wasm result. Actual source/producer/native acceptance is separate.
 */
public final class OfficialSourceCompatTest {
    public static void main(String[] args) {
        final LocalPolicyTest.Model model=new LocalPolicyTest.Model();
        Deployment.Binding[] bindings={OfficialBootstrap.topic("/minmal_publisher",true),OfficialBootstrap.clock("/minmal_publisher"),OfficialBootstrap.topic("/minimal_subscriber",false),OfficialBootstrap.topic("/publisher_node",true),OfficialBootstrap.clock("/publisher_node"),OfficialBootstrap.topic("/subscriber_node",false),OfficialBootstrap.clock("/minimal_timer")};
        Deployment.install(new Deployment.Profile(model,bindings,()->{model.clock+=500000000L;return true;},64,256,256,4));
        RCLJava.rclJavaInit();Throwable primary=null;
        try {
            ComposableNode[] nodes={new org.ros2.rcljava.examples.publisher.PublisherLambda(),new org.ros2.rcljava.examples.publisher.PublisherMemberFunction(),new org.ros2.rcljava.examples.subscriber.SubscriberLambda(),new org.ros2.rcljava.examples.subscriber.SubscriberMemberFunction(),new org.ros2.rcljava.examples.composition.PublisherNode(),new org.ros2.rcljava.examples.composition.SubscriberNode(),new org.ros2.rcljava.examples.timer.TimerLambda(),new org.ros2.rcljava.examples.timer.TimerMemberFunction()};
            // Also require the unchanged non-composable subscriber and composed
            // entry classes during real compilation, without invoking main here.
            Class<?>[] entries={org.ros2.rcljava.examples.subscriber.SubscriberNotComposable.class,org.ros2.rcljava.examples.composition.Composed.class};
            LocalPolicyTest.check(entries.length==2&&nodes.length==8);
            SingleThreadedExecutor executor=new SingleThreadedExecutor();for(ComposableNode node:nodes)executor.addNode(node);
            model.poll=new Result(0,Codecs.STRING.encode("official-source-local-model"),null,null);
            executor.spin();executor.spinSome();
            LocalPolicyTest.check(!model.published.isEmpty());
            for(byte[] value:model.published)LocalPolicyTest.check(Codecs.STRING.decode(value).startsWith("Hello, world! "));
            LocalPolicyTest.unsupported(()->executor.spinOnce(1));LocalPolicyTest.unsupported(()->executor.spinAll(0));
            LocalPolicyTest.unsupported(()->new org.ros2.rcljava.executors.MultiThreadedExecutor());
            for(ComposableNode node:nodes)executor.removeNode(node);
        } catch(RuntimeException|Error error) { primary=error;throw error; }
        finally { Deployment.shutdown(primary); }
        LocalPolicyTest.check(!RCLJava.ok());
        System.out.println("PASS PINNED_OFFICIAL_SOURCE_COMPAT_MODEL_ONLY checks="+LocalPolicyTest.checks+" native_ros_cases=0");
    }
}
