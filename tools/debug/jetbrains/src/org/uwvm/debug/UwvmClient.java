package org.uwvm.debug;

import com.intellij.openapi.Disposable;
import com.intellij.openapi.project.Project;
import com.intellij.openapi.util.Disposer;
import com.intellij.xdebugger.*;
import com.redhat.devtools.lsp4ij.dap.*;
import com.redhat.devtools.lsp4ij.dap.client.DAPClient;
import com.redhat.devtools.lsp4ij.settings.ServerTrace;
import org.eclipse.lsp4j.debug.services.IDebugProtocolServer;
import org.eclipse.lsp4j.jsonrpc.*;
import java.io.*;
import java.util.*;
import java.util.concurrent.*;
import java.util.function.UnaryOperator;

/** One authenticated adapter endpoint per actual selected UWVM IDE session. */
public final class UwvmClient extends DAPClient {
    public interface Protocol extends IDebugProtocolServer {
        @org.eclipse.lsp4j.jsonrpc.services.JsonRequest("uwvm/permissions")
        CompletableFuture<com.google.gson.JsonObject> permissions(Map<String,Object> arguments);
        @org.eclipse.lsp4j.jsonrpc.services.JsonRequest("uwvm/diagnostics")
        CompletableFuture<com.google.gson.JsonObject> diagnostics(Map<String,Object> arguments);
        @org.eclipse.lsp4j.jsonrpc.services.JsonRequest("uwvm/dump")
        CompletableFuture<com.google.gson.JsonObject> dump(Map<String,Object> arguments);
        @org.eclipse.lsp4j.jsonrpc.services.JsonRequest("uwvm/memoryTags")
        CompletableFuture<com.google.gson.JsonObject> memoryTags(Map<String,Object> arguments);
        @org.eclipse.lsp4j.jsonrpc.services.JsonRequest("uwvm/hostTrace")
        CompletableFuture<com.google.gson.JsonObject> hostTrace(Map<String,Object> arguments);
        @org.eclipse.lsp4j.jsonrpc.services.JsonRequest("uwvm/hostTraceBytes")
        CompletableFuture<com.google.gson.JsonObject> hostTraceBytes(Map<String,Object> arguments);
        @org.eclipse.lsp4j.jsonrpc.services.JsonRequest("uwvm/exportHostTrace")
        CompletableFuture<com.google.gson.JsonObject> exportHostTrace(Map<String,Object> arguments);
    }
    private static final Map<Project,UwvmClient> clients=new ConcurrentHashMap<>();
    private final DAPDebugProcess process;
    private volatile RemoteEndpoint endpoint;
    UwvmClient(DAPDebugProcess process,Map<String,Object> arguments,boolean debug,
            ServerTrace trace,DAPClient parent) {
        super(process,arguments,debug,DebugMode.ATTACH,trace,parent);this.process=process;
        Project project=process.getSession().getProject();clients.put(project,this);
        Disposer.register(project,(Disposable)()->clients.remove(project,this));
    }
    @Override protected Launcher<? extends IDebugProtocolServer> createLauncher(
            UnaryOperator<MessageConsumer> wrapper,InputStream input,OutputStream output,ExecutorService threads) {
        var launcher=new org.eclipse.lsp4j.jsonrpc.debug.DebugLauncher.Builder<Protocol>()
            .setLocalService(this).setRemoteInterface(Protocol.class).setInput(input).setOutput(output)
            .setExecutorService(threads).wrapMessages(wrapper).create();
        endpoint=launcher.getRemoteEndpoint();return launcher;
    }
    public static UwvmClient current(Project project) {
        var client=clients.get(project);
        var session=XDebuggerManager.getInstance(project).getCurrentSession();
        if(client==null || session!=client.process.getSession() || session.isStopped())
            throw new IllegalStateException("Select the live UWVM debug session");
        return client;
    }
    public CompletableFuture<Object> inspect(String method,Map<String,Object> arguments) {
        if(!Set.of("uwvm/memoryTags","uwvm/hostTrace","uwvm/hostTraceBytes","uwvm/exportHostTrace","uwvm/permissions","uwvm/diagnostics","uwvm/dump").contains(method))
            return CompletableFuture.failedFuture(new IllegalArgumentException("Unsupported inspection request"));
        boolean metadata=Set.of("uwvm/permissions","uwvm/diagnostics","uwvm/dump").contains(method);
        if(current(process.getSession().getProject())!=this || (!metadata && !process.getSession().isPaused()) || endpoint==null)
            return CompletableFuture.failedFuture(new IllegalStateException("Pause the selected UWVM session before inspection"));
        return endpoint.request(method,arguments).thenApply(value->{
            if(current(process.getSession().getProject())!=this || (!metadata && !process.getSession().isPaused()))
                throw new IllegalStateException("UWVM session resumed or retired during inspection");
            return value;
        });
    }
}
