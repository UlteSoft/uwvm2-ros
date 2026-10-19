package org.ros2.rcljava;
import org.ros2.rcljava.contexts.Context;
import org.ros2.rcljava.executors.SingleThreadedExecutor;
import org.ros2.rcljava.migration.Deployment;
import org.ros2.rcljava.migration.MigrationContext;
import org.ros2.rcljava.node.ComposableNode;
import org.ros2.rcljava.node.Node;
import org.ros2.rcljava.node.NodeOptions;
/** Opt-in pinned rcljava source migration entry, never a JNI/rcl replacement.
 * An unchanged business entry follows an independent Deployment bootstrap.
 * Unsupported parameter/remapping/native RMW/context-pointer semantics are
 * refused, not silently accepted. Use a fresh Guest for each lifecycle/test.
 */
public final class RCLJava {
    private static MigrationContext context;
    private static SingleThreadedExecutor executor;
    private RCLJava() {}
    public static synchronized void installDeployment(Deployment.Profile profile) {
        if(context!=null&&context.wasInitialized())throw new IllegalStateException("install before initialization");
        if(executor!=null)throw new IllegalStateException("executor already selected");
        context=new MigrationContext(profile);
    }
    public static synchronized MigrationContext migrationContext() {
        if(context==null)context=new MigrationContext(Deployment.Profile.denied());
        return context;
    }
    public static Context getDefaultContext() { return migrationContext(); }
    public static void rclJavaInit() { migrationContext().init(); }
    public static boolean ok() { return context!=null&&context.isValid(); }
    public static boolean ok(Context selected) { return selected!=null&&selected.isValid(); }
    @Deprecated public static boolean isInitialized() { return ok(); }
    public static Context createContext() { throw new UnsupportedOperationException("additional native context semantics are not implemented"); }
    public static Node createNode(String name) { return migrationContext().createNode(name,""); }
    public static Node createNode(String name,String namespace,Context selected) {
        if(selected!=migrationContext())throw new IllegalArgumentException("foreign context");
        return context.createNode(name,namespace);
    }
    public static Node createNode(String name,String namespace,Context selected,NodeOptions options) {
        throw new UnsupportedOperationException("NodeOptions/remapping/automatic rosout/parameter policies require complete implementation");
    }
    public static String getRMWIdentifier() { throw new UnsupportedOperationException("native RMW identity is independently qualified, not inferred from the Wasm bridge"); }
    public static synchronized void shutdown() { if(context!=null&&context.isValid())context.shutdown(); }
    private static SingleThreadedExecutor global() { if(executor==null)executor=new SingleThreadedExecutor();return executor; }
    private static ComposableNode compose(final Node node) { if(node==null)throw new IllegalArgumentException("node");return new ComposableNode(){public Node getNode(){return node;}}; }
    private static void drive(ComposableNode node,int mode) {
        SingleThreadedExecutor current=global();current.addNode(node);Throwable primary=null;
        try { if(mode==0)current.spin();else if(mode==1)current.spinOnce();else current.spinSome(); }
        catch(RuntimeException|Error failure) { primary=failure;throw failure; }
        finally {
            try { current.removeNode(node); }
            catch(RuntimeException|Error cleanup) {
                if(primary==null)throw cleanup;
                if(cleanup!=primary)try { primary.addSuppressed(cleanup); }catch(RuntimeException|Error unavailable) {}
            }
        }
    }
    public static void spin(Node node) { drive(compose(node),0); }
    public static void spin(ComposableNode node) { drive(node,0); }
    public static void spinOnce(Node node) { drive(compose(node),1); }
    public static void spinOnce(ComposableNode node) { drive(node,1); }
    public static void spinSome(Node node) { drive(compose(node),2); }
    public static void spinSome(ComposableNode node) { drive(node,2); }
}
