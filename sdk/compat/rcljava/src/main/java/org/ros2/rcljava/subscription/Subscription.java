package org.ros2.rcljava.subscription;
import java.lang.ref.WeakReference;
import org.ros2.rcljava.node.Node;
import org.ros2.rcljava.interfaces.Disposable;
import org.ros2.rcljava.interfaces.MessageDefinition;
/** Pinned typed callback subset; a native thread never enters the Guest. */
public interface Subscription<T extends MessageDefinition> extends Disposable {
    Class<T> getMessageType();
    WeakReference<Node> getNodeReference();
    void executeCallback(T message);
}
