package org.uwvm.roswasm.examples;

import org.uwvm.roswasm.*;

/** Real TeaVM WEBASSEMBLY_WASI default-authority lesson.
 * Select Ros1Transport for a ROS1-only binary or Ros2Transport for ROS2.
 * No grants are supplied. Missing imports/producer failures are not deny13.
 */
public final class DefaultDenied {
    public static void main(String[] args){
        Transport backend=new Ros2Transport();
        try(Session session=new Session(backend,4)){
            session.checkAbi();
            try{session.open("ros-sdk-denied");throw new IllegalStateException("default grant unexpectedly opened");}
            catch(RosException refusal){if(refusal.result.error!=13)throw refusal;}
        }
        System.out.println("PASS ROSWASM_JAVA_DEFAULT_DENIED13");
    }
}
