package org.uwvm.rcljava;
import org.ros2.rcljava.RCLJava;
import org.ros2.rcljava.migration.Deployment;
import org.ros2.rcljava.migration.MigrationContext;
import org.ros2.rcljava.node.Node;
import org.uwvm.roswasm.Ros2Transport;
import org.uwvm.roswasm.RosException;
/** Real target entry: selected local name-to-alias binding, EMPTY Host grants.
 * This deliberately reaches actual capability_open. An empty local mapping's
 * guest-side13 would be a useful policy model but not Host-denial evidence.
 */
public final class DefaultDeniedBootstrap {
    public static void main(String[] args) {
        Deployment.Binding binding=new Deployment.Binding("/denied","/topic","std_msgs/msg/String","official-default-profile","topic_publish","rcljava-denied",0);
        Deployment.install(new Deployment.Profile(new Ros2Transport(),new Deployment.Binding[]{binding},null,4,8,4,0));
        RCLJava.rclJavaInit();Throwable primary=null;
        try {
            Node node=RCLJava.createNode("denied");boolean denied=false;
            try { node.createPublisher(std_msgs.msg.String.class,"topic"); }
            catch(RosException refusal) {
                if(refusal.result.error!=13)throw refusal;
                MigrationContext.CleanupAttempt report=Deployment.context().lastFactoryCleanup();
                if(report==null||report.primary!=refusal||report.attempted||!node.getPublishers().isEmpty())throw new IllegalStateException("unexpected denied-open ownership state");
                denied=true;
            }
            if(!denied)throw new IllegalStateException("actual Host unexpectedly granted publisher");
        } catch(RuntimeException|Error failure) { primary=failure;throw failure; }
        finally { Deployment.shutdown(primary); }
        System.out.println("PASS RCLJAVA_ACTUAL_IMPORTS_HOST_DENIED13_REQUIRES_INDEPENDENT_RUNTIME_RECEIPT");
    }
}
