import java.io.IOException;
import org.uwvm.roswasm.*;
import org.uwvm.roswasm.generated.BoundedRecords.Ros2Sensor_msgsMsgJointState;

/** Real ROS2 SDK code. Exact aliases/native identity/QoS/peers belong to Root. */
public final class JointStateExample {
    public static void main(String[] args) throws IOException {
        if(args.length!=2)throw new IllegalArgumentException("publish and subscribe aliases required");
        boolean[] seen={false};
        try(Session session=new Session(new Ros2Transport(),2)) {
            session.checkAbi();Node node=new Node(session);
            Node.Subscription<Ros2Sensor_msgsMsgJointState> subscription=node.createSubscription(
                Ros2Sensor_msgsMsgJointState.CODEC,args[1],Ros2Sensor_msgsMsgJointState.MAX_WIRE_BYTES,value->{
                    if(value.header.stamp.sec!=1||value.header.stamp.nanosec!=2||
                       !value.header.frame_id.equals("managed")||value.name.length!=1||!value.name[0].equals("joint1")||
                       value.position.length!=1||value.position[0]!=1.25||value.velocity.length!=1||value.velocity[0]!=0.5||
                       value.effort.length!=1||value.effort[0]!=-0.75)
                        throw new IllegalStateException("complete native/Guest value differs");
                    seen[0]=true;
                });
            Node.Publisher<Ros2Sensor_msgsMsgJointState> publisher=node.createPublisher(Ros2Sensor_msgsMsgJointState.CODEC,args[0]);
            Ros2Sensor_msgsMsgJointState value=new Ros2Sensor_msgsMsgJointState();
            value.header.stamp.sec=1;value.header.stamp.nanosec=2;value.header.frame_id="managed";
            value.name=new String[]{"joint1"};value.position=new double[]{1.25};
            value.velocity=new double[]{0.5};value.effort=new double[]{-0.75};
            publisher.publish(value).checked();
            for(int i=0;i<256&&!seen[0];++i) {
                int credit=System.in.read();
                if(credit=='R') { Result r=subscription.pollOnce();if(r.error!=0&&r.error!=11)r.checked(); }
                else if(credit=='Q')break;
                else throw new IllegalArgumentException("bounded explicit R/Q stdin schedule required");
            }
            if(!seen[0])throw new IllegalStateException("no complete independently witnessed value; no replay");
        }
    }
}
