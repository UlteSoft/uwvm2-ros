package org.uwvm.debug;

import com.intellij.execution.ExecutionException;
import com.intellij.execution.configurations.GeneralCommandLine;
import com.intellij.execution.configurations.RunConfigurationOptions;
import com.intellij.execution.runners.ExecutionEnvironment;
import com.redhat.devtools.lsp4ij.dap.DAPDebugProcess;
import com.redhat.devtools.lsp4ij.dap.DebugMode;
import com.redhat.devtools.lsp4ij.dap.client.DAPClient;
import com.redhat.devtools.lsp4ij.dap.configurations.DAPRunConfigurationOptions;
import com.redhat.devtools.lsp4ij.dap.descriptors.*;
import com.redhat.devtools.lsp4ij.settings.ServerTrace;
import com.google.gson.*;
import java.nio.file.Path;
import java.util.*;

/** Starts an authenticated stdio adapter, then sends DAP attach, never native PID attach. */
public final class UwvmDescriptorFactory extends DebugAdapterDescriptorFactory {
    @Override public DebugAdapterDescriptor createDebugAdapterDescriptor(
            RunConfigurationOptions options, ExecutionEnvironment environment) {
        return new DefaultDebugAdapterDescriptor(options, environment, getServerDefinition()) {
            private Map<String,Object> parameters() {
                if (!(options instanceof DAPRunConfigurationOptions dap))
                    throw new IllegalArgumentException("UWVM requires its generated DAP configuration");
                JsonElement document=JsonParser.parseString(dap.getAttachConfiguration());
                if (!document.isJsonObject()) throw new IllegalArgumentException("UWVM attach settings must be an object");
                if (document.getAsJsonObject().has("debugPermissions") &&
                    "none".equals(document.getAsJsonObject().get("debugPermissions").getAsString()))
                    throw new IllegalArgumentException("Debugger disabled; run the generated runtime task without attaching");
                return object(document.getAsJsonObject());
            }
            @Override public GeneralCommandLine createStartServerCommandLine(RunConfigurationOptions ignored)
                    throws ExecutionException {
                Map<String,Object> args=parameters();
                var command=new GeneralCommandLine(path(args,"uwvmPython"),path(args,"uwvmImporter"),
                        "adapter","--profile",path(args,"uwvmProfile"));
                // This is a generic DAP client; the VS Code-only URI provider is absent.
                var inherited=new HashMap<>(System.getenv());inherited.remove("UWVM_DAP_SOURCE_SESSION");
                command.withParentEnvironmentType(GeneralCommandLine.ParentEnvironmentType.NONE).withEnvironment(inherited);
                return command;
            }
            // LSP4IJ 0.21 treats ATTACH as an already-running TCP adapter.
            // Launch only our stdio transport process; the protocol client below
            // sends attach to the existing, independently authorized broker.
            @Override public DebugMode getDebugMode() { return DebugMode.LAUNCH; }
            @Override public ServerReadyConfig getServerReadyConfig(DebugMode ignored) { return new ServerReadyConfig(0); }
            @Override public Map<String,Object> getDapParameters() {
                var args=parameters();args.remove("uwvmPython");args.remove("uwvmImporter");args.remove("uwvmProfile");
                return args;
            }
            @Override public DAPClient createClient(DAPDebugProcess process,Map<String,Object> args,boolean debug,
                    DebugMode ignored,ServerTrace trace,DAPClient parent) {
                return new UwvmClient(process,args,debug,trace,parent);
            }
        };
    }
    private static String path(Map<String,Object> values,String key) {
        Object raw=values.get(key);
        if (!(raw instanceof String text) || !Path.of(text).isAbsolute() ||
                text.chars().anyMatch(c -> c<32)) throw new IllegalArgumentException(key+" must be an absolute host path");
        return text;
    }
    private static Map<String,Object> object(JsonObject value) {
        Map<String,Object> result=new LinkedHashMap<>();
        value.entrySet().forEach(entry -> result.put(entry.getKey(),decode(entry.getValue())));
        return result;
    }
    private static Object decode(JsonElement value) {
        if(value.isJsonObject()) return object(value.getAsJsonObject());
        if(value.isJsonArray()) { List<Object> out=new ArrayList<>();value.getAsJsonArray().forEach(v -> out.add(decode(v)));return out; }
        if(value.isJsonNull()) return null;
        JsonPrimitive primitive=value.getAsJsonPrimitive();
        if(primitive.isBoolean()) return primitive.getAsBoolean();
        if(primitive.isString()) return primitive.getAsString();
        // Preserve integer JSON spellings; do not round module IDs through double.
        return new java.math.BigDecimal(primitive.getAsString());
    }
}
