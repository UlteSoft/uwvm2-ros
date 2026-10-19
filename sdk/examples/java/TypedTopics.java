package org.uwvm.roswasm.examples;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.nio.charset.StandardCharsets;
import org.uwvm.roswasm.*;

/** Real bounded topic lesson. Host grants two independent String aliases.
 * An independent native observer must verify the publication. At most64 stdin
 * R credits drive guest callbacks; EOF terminates without background work.
 */
public final class TypedTopics {
    public static void main(String[] args) throws Exception {
        try(Session session=new Session(new Ros2Transport(),8)){
            session.checkAbi();Node node=new Node(session);String[] received={null};
            Node.Subscription<String> subscription=node.createSubscription(Codecs.STRING,"subscribe",4096,value->{if(received[0]!=null)throw new IllegalStateException("duplicate message");received[0]=value;});
            Node.Publisher<String> publisher=node.createPublisher(Codecs.STRING,"publish");
            publisher.publish("roswasm-java:机器人").checked();
            Node.Executor executor=new Node.Executor(1);executor.add(subscription);
            BufferedReader input=new BufferedReader(new InputStreamReader(System.in,StandardCharsets.UTF_8));
            for(int i=0;i<64 && received[0]==null;++i){
                if(!"R".equals(input.readLine()))break;
                Result r=executor.spinOnce();if(r.error!=0&&r.error!=11)r.checked();
            }
            if(!"roswasm-java:机器人".equals(received[0]))throw new IllegalStateException("native delivery missing/mismatched");
        }
        System.out.println("PASS ROSWASM_JAVA_TOPIC_ROUNDTRIP");
    }
}
