package org.ros2.rcljava.publisher;
import java.lang.ref.WeakReference;
import org.ros2.rcljava.node.Node;
import org.ros2.rcljava.interfaces.Disposable;
import org.ros2.rcljava.interfaces.MessageDefinition;
/** Pinned publish/dispose/source-reference signatures. Native QoS event-handler
 * types and callbacks remain unimplemented; no dummy EventHandler is provided.
 */
public interface Publisher<T extends MessageDefinition> extends Disposable {
    void publish(T message);
    WeakReference<Node> getNodeReference();
}
