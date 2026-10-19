package org.ros2.rcljava.executors;
import org.ros2.rcljava.RCLJava;
import org.ros2.rcljava.migration.MigrationContext;
import org.ros2.rcljava.node.ComposableNode;
import org.ros2.rcljava.node.Node;
/** Bounded single-Guest-thread topic/timer executor, not a native wait set.
 * Explicit timeout0 is nonblocking. Default/infinite spin requires the selected
 * finite deployment driver; positive timeouts and spinAll are refused BEFORE
 * polling. No System.nanoTime, sleep, thread pool or hidden clock is used.
 */
public class SingleThreadedExecutor implements Executor {
    private final MigrationContext context;
    private final ComposableNode[] registrations;
    private final Node[] nodes;
    private int position;
    private boolean spinning;
    public SingleThreadedExecutor() {
        context=RCLJava.migrationContext();
        registrations=new ComposableNode[context.maximumNodes()];nodes=new Node[registrations.length];
    }
    private void mutable() { if(spinning)throw new IllegalStateException("executor is active");context.requireMutable(); }
    public void addNode(ComposableNode value) {
        mutable();if(value==null)throw new IllegalArgumentException("node");
        Node node=value.getNode();int slot=-1;
        for(int i=0;i<nodes.length;++i) { if(registrations[i]==value||nodes[i]==node)throw new IllegalArgumentException("duplicate node");if(slot<0&&nodes[i]==null)slot=i; }
        if(slot<0)throw new IllegalStateException("executor node capacity");
        context.attachExecutor(node,this);registrations[slot]=value;nodes[slot]=node;
    }
    public void removeNode(ComposableNode value) {
        if(spinning)throw new IllegalStateException("executor is active");context.requireLocalMutable();
        for(int i=0;i<registrations.length;++i)if(registrations[i]==value) { context.detachExecutor(nodes[i],this);registrations[i]=null;nodes[i]=null;return; }
    }
    private void one() {
        for(int count=0;count<nodes.length;++count) {
            Node node=nodes[position];position=(position+1)%nodes.length;
            if(node!=null&&context.poll(node,this)!=11)return;
        }
    }
    public void spinOnce() { spinOnce(-1); }
    public void spinOnce(long timeout) {
        mutable();if(timeout!=0&&timeout!=-1)throw new UnsupportedOperationException("native timed wait-set semantics are not implemented");
        spinning=true;
        try { if(timeout==0||context.nextCredit())one(); } finally { spinning=false; }
    }
    public void spinSome() { spinSome(0); }
    public void spinSome(long maximumDurationNs) {
        mutable();if(maximumDurationNs!=0)throw new UnsupportedOperationException("duration-bound native spinSome requires a native wait set");
        spinning=true;
        try {
            // One exact fixed-table snapshot: every current source is polled
            // at most once. Callback factories/removals are leased out.
            for(int count=0;count<nodes.length;++count) {
                Node node=nodes[position];position=(position+1)%nodes.length;
                if(node!=null)context.pollSnapshot(node,this);
            }
        } finally { spinning=false; }
    }
    public void spinAll(long maximumDurationNs) { throw new UnsupportedOperationException("native exhaustive/duration wait-set semantics are not implemented"); }
    public void spin() {
        mutable();if(!context.hasDriver())throw new UnsupportedOperationException("install an explicit finite deployment driver before blocking spin");
        spinning=true;
        try { while(context.isValid()&&context.nextCredit())one(); } finally { spinning=false; }
    }
}
