package org.uwvm.debug;

import com.google.gson.*;
import java.awt.event.*;
import java.io.*;
import java.nio.file.*;
import java.util.*;
import javax.swing.JFileChooser;
import org.netbeans.modules.lsp.client.debugger.api.DAPConfiguration;
import org.openide.awt.*;
import org.openide.util.Exceptions;
import org.openide.util.RequestProcessor;

/** Host-selected configuration; the existing Python broker authenticates the VM. */
@ActionID(category="Debug", id="org.uwvm.debug.Attach")
@ActionRegistration(displayName="Attach UWVM Wasm...")
@ActionReference(path="Menu/RunProject", position=2190)
public final class Attach implements ActionListener {
    @Override public void actionPerformed(ActionEvent ignored) {
        var chooser=new JFileChooser();
        chooser.setDialogTitle("Select generated UWVM NetBeans configuration");
        if(chooser.showOpenDialog(null)==JFileChooser.APPROVE_OPTION)
            RequestProcessor.getDefault().post(() -> {
                try {start(chooser.getSelectedFile().toPath());}
                catch(Exception error) {Exceptions.printStackTrace(error);}
            });
    }
    public static Process start(Path configuration) throws Exception {
        JsonObject document;
        try(var reader=Files.newBufferedReader(configuration)) {
            document=JsonParser.parseReader(reader).getAsJsonObject();
        }
        if (document.has("attach") && document.getAsJsonObject("attach").has("debugPermissions") &&
            "none".equals(document.getAsJsonObject("attach").get("debugPermissions").getAsString()))
            throw new IllegalArgumentException("Debugger disabled; run the generated runtime task without attaching");
        if(!"uwvm-netbeans-attach-v1".equals(document.get("format").getAsString()))
            throw new IllegalArgumentException("Select a generated UWVM NetBeans configuration");
        String python=path(document,"python"),importer=path(document,"importer"),profile=path(document,"profile");
        var validation=new ProcessBuilder(python,importer,"validate","--profile",profile)
            .redirectOutput(ProcessBuilder.Redirect.DISCARD).redirectError(ProcessBuilder.Redirect.INHERIT).start();
        if(validation.waitFor()!=0)throw new IllegalArgumentException("Host profile validation failed");
        var builder=new ProcessBuilder(python,importer,"adapter","--profile",profile)
            .redirectError(ProcessBuilder.Redirect.INHERIT);
        builder.environment().remove("UWVM_DAP_SOURCE_SESSION");
        var child=builder.start();
        var shutdown=new java.lang.Thread(child::destroy,"UWVM adapter shutdown");
        Runtime.getRuntime().addShutdownHook(shutdown);
        child.onExit().thenRun(() -> {try{Runtime.getRuntime().removeShutdownHook(shutdown);}catch(IllegalStateException ignored){}});
        try {
            DAPConfiguration.create(child.getInputStream(),child.getOutputStream())
                .setSessionName(document.get("name").getAsString())
                .addConfiguration(object(document.getAsJsonObject("attach"))).attach();
            return child;
        } catch(RuntimeException failure) {child.destroy();throw failure;}
    }
    private static String path(JsonObject document,String key) {
        String value=document.get(key).getAsString();
        if(!Path.of(value).isAbsolute() || value.chars().anyMatch(c->c<32))
            throw new IllegalArgumentException(key+" must be an absolute host path");
        return value;
    }
    private static Map<String,Object> object(JsonObject value) {
        Map<String,Object> result=new LinkedHashMap<>();
        value.entrySet().forEach(entry->result.put(entry.getKey(),decode(entry.getValue())));
        return result;
    }
    private static Object decode(JsonElement value) {
        if(value.isJsonObject())return object(value.getAsJsonObject());
        if(value.isJsonArray()){List<Object> result=new ArrayList<>();value.getAsJsonArray().forEach(v->result.add(decode(v)));return result;}
        if(value.isJsonNull())return null;
        var p=value.getAsJsonPrimitive();
        if(p.isBoolean())return p.getAsBoolean();
        if(p.isString())return p.getAsString();
        return new java.math.BigDecimal(p.getAsString());
    }
}
