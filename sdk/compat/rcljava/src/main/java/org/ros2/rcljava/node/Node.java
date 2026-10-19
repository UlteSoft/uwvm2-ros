package org.ros2.rcljava.node;

import java.util.Collection;
import java.util.concurrent.TimeUnit;
import org.ros2.rcljava.concurrent.Callback;
import org.ros2.rcljava.consumers.Consumer;
import org.ros2.rcljava.interfaces.Disposable;
import org.ros2.rcljava.interfaces.MessageDefinition;
import org.ros2.rcljava.interfaces.ServiceDefinition;
import org.ros2.rcljava.consumers.TriConsumer;
import org.ros2.rcljava.service.RMWRequestId;
import org.ros2.rcljava.service.Service;
import org.ros2.rcljava.client.Client;
import org.ros2.rcljava.publisher.Publisher;
import org.ros2.rcljava.subscription.Subscription;
import org.ros2.rcljava.timer.Timer;
import org.ros2.rcljava.timer.WallTimer;
import org.ros2.rcljava.qos.QoSProfile;

/** Explicit topic/timer and bounded Service/Future subset of the pinned official Node interface.
 * Parameters, graph, service availability/Action discovery and native-pointer operations are
 * deliberately absent from this opt-in profile. They must not compile as no-op
 * success. The complete original interface is preserved under upstream/.
 */
public interface Node extends Disposable {
    <T extends MessageDefinition> Publisher<T> createPublisher(Class<T> type, String topic);
    <T extends MessageDefinition> Publisher<T> createPublisher(Class<T> type, String topic, QoSProfile qos);
    <T extends MessageDefinition> Subscription<T> createSubscription(Class<T> type, String topic, Consumer<T> callback);
    <T extends MessageDefinition> Subscription<T> createSubscription(Class<T> type, String topic, Consumer<T> callback, QoSProfile qos);
    WallTimer createWallTimer(long period, TimeUnit unit, Callback callback);
    Collection<Publisher> getPublishers();
    Collection<Subscription> getSubscriptions();
    Collection<Timer> getTimers();
    boolean removePublisher(Publisher publisher);
    boolean removeSubscription(Subscription subscription);
    <T extends ServiceDefinition> Service<T> createService(Class<T> type,String name,TriConsumer<RMWRequestId,? extends MessageDefinition,? extends MessageDefinition> callback) throws NoSuchFieldException,IllegalAccessException;
    <T extends ServiceDefinition> Service<T> createService(Class<T> type,String name,TriConsumer<RMWRequestId,? extends MessageDefinition,? extends MessageDefinition> callback,QoSProfile qos) throws NoSuchFieldException,IllegalAccessException;
    <T extends ServiceDefinition> Client<T> createClient(Class<T> type,String name) throws NoSuchFieldException,IllegalAccessException;
    <T extends ServiceDefinition> Client<T> createClient(Class<T> type,String name,QoSProfile qos) throws NoSuchFieldException,IllegalAccessException;
    Collection<Service> getServices();
    Collection<Client> getClients();
    boolean removeService(Service service);
    boolean removeClient(Client client);
    String getName();
    String getNamespace();
}
