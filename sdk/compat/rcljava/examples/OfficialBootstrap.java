package org.uwvm.rcljava;
import org.ros2.rcljava.migration.Deployment;
import org.uwvm.roswasm.Ros2Transport;
/** Deployment entry selected INSTEAD OF modifying the preserved official app.
 * These alias strings configure no Host authority. A real selected ROS2 Host
 * policy/native peer must independently bind each original name/type/profile.
 * All modes are fresh Guest instances. Stdin provides <=64 R-newline credits.
 */
public final class OfficialBootstrap {
    static Deployment.Binding topic(String node,boolean publish) {
        return new Deployment.Binding(node,"/topic","std_msgs/msg/String","official-default-profile",publish?"topic_publish":"topic_subscribe",publish?"publish":"subscribe",publish?0:65536);
    }
    static Deployment.Binding clock(String node) { return new Deployment.Binding(node,node,"","","time_read_steady","steady",8); }
    public static void main(String[] args) throws Exception {
        if(args.length!=1)throw new IllegalArgumentException("publisher|publisher-member|subscriber|subscriber-member|subscriber-node|composed|timer|timer-member");
        Deployment.Binding[] bindings={topic("/minmal_publisher",true),clock("/minmal_publisher"),topic("/minimal_subscriber",false),topic("/publisher_node",true),clock("/publisher_node"),topic("/subscriber_node",false),clock("/minimal_timer")};
        Deployment.install(new Deployment.Profile(new Ros2Transport(),bindings,new Deployment.StdinCredits(System.in,64),64,256,256,64));
        Throwable primary=null;
        try {
            String[] originalArguments=new String[0];
            switch(args[0]) {
            case "publisher": org.ros2.rcljava.examples.publisher.PublisherLambda.main(originalArguments);break;
            case "publisher-member": org.ros2.rcljava.examples.publisher.PublisherMemberFunction.main(originalArguments);break;
            case "subscriber": org.ros2.rcljava.examples.subscriber.SubscriberLambda.main(originalArguments);break;
            case "subscriber-member": org.ros2.rcljava.examples.subscriber.SubscriberMemberFunction.main(originalArguments);break;
            case "subscriber-node": org.ros2.rcljava.examples.subscriber.SubscriberNotComposable.main(originalArguments);break;
            case "composed": org.ros2.rcljava.examples.composition.Composed.main(originalArguments);break;
            case "timer": org.ros2.rcljava.examples.timer.TimerLambda.main(originalArguments);break;
            case "timer-member": org.ros2.rcljava.examples.timer.TimerMemberFunction.main(originalArguments);break;
            default: throw new IllegalArgumentException("unselected official example");
            }
        } catch(Exception|Error error) { primary=error;throw error; }
        finally { Deployment.shutdown(primary); }
    }
}
