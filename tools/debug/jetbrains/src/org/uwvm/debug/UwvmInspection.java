package org.uwvm.debug;

import com.intellij.openapi.project.Project;
import com.intellij.openapi.wm.*;
import com.intellij.ui.content.ContentFactory;
import com.google.gson.*;
import java.awt.*;
import java.math.BigInteger;
import java.util.*;
import java.util.function.Consumer;
import javax.swing.*;
import javax.swing.table.*;

/** Read-only, paged host inspection. All authority remains in the broker/VM. */
public final class UwvmInspection implements ToolWindowFactory, com.intellij.openapi.project.DumbAware {
    private static final Gson JSON=new GsonBuilder().setPrettyPrinting().create();
    private static final BigInteger SIXTEEN=BigInteger.valueOf(16);
    @Override public void createToolWindowContent(Project project,ToolWindow window) {
        var tabs=new JTabbedPane();
        tabs.add("Diagnostics / dump",new DiagnosticsPanel(project));
        tabs.add("Memory tags",new MemoryPanel(project));
        tabs.add("Host calls",new HostPanel(project));
        window.getContentManager().addContent(ContentFactory.getInstance().createContent(tabs,"",false));
    }
    private static void request(Project project,String method,Map<String,Object> args,JLabel state,Consumer<JsonObject> result) {
        try {
            state.setText("Reading the current paused snapshot...");
            UwvmClient.current(project).inspect(method,args).whenComplete((value,error)->SwingUtilities.invokeLater(()->{
                if(error!=null){state.setText("Unavailable: "+error.getMessage());return;}
                try {result.accept(JSON.toJsonTree(value).getAsJsonObject());}
                catch(RuntimeException invalid){state.setText("Invalid or retired snapshot: "+invalid.getMessage());}
            }));
        }catch(RuntimeException error){state.setText("Unavailable: "+error.getMessage());}
    }
    private static BigInteger unsigned(String text,int width) {
        if(!text.matches("[0-9]+"))throw new IllegalArgumentException("Use unsigned decimal selectors");
        var value=new BigInteger(text);if(value.bitLength()>width)throw new IllegalArgumentException("Selector exceeds its address width");return value;
    }
    private static JTextField field(String name,String value,int width) {
        var field=new JTextField(value,width);field.setName(name);return field;
    }
    public static final class DiagnosticsPanel extends JPanel {
        DiagnosticsPanel(Project project) {
            super(new BorderLayout());var state=new JLabel("Permissions are fixed by the VM launch; limited exports are redacted.");
            var text=new JTextArea();text.setEditable(false);var controls=new JPanel();
            var read=new JButton("Read diagnostics / permissions");
            read.addActionListener(event->request(project,"uwvm/diagnostics",Map.of(),state,r->{
                text.setText(JSON.toJson(r));state.setText("Debugger permission: "+r.get("permissions").getAsString());}));controls.add(read);
            var stop=field("uwvm.dumpStop","1",12);controls.add(new JLabel("Current stop ID"));controls.add(stop);
            var save=new JButton("Save Wasm dump");save.addActionListener(event->{
                String path=JOptionPane.showInputDialog(this,"Absolute output path on the VM host; existing files are refused");
                if(path!=null)try {
                    var id=unsigned(stop.getText(),64);if(id.signum()==0)throw new IllegalArgumentException("Current stop ID required");
                    request(project,"uwvm/dump",Map.of("stopId",id.toString(),"path",path),state,r->{text.setText(JSON.toJson(r));state.setText("Dump exported with current permissions");});
                } catch(RuntimeException invalid){state.setText(invalid.getMessage());}
            });controls.add(save);add(controls,BorderLayout.NORTH);add(new JScrollPane(text),BorderLayout.CENTER);add(state,BorderLayout.SOUTH);
        }
    }
    public static final class MemoryPanel extends JPanel {
        private final Project project;
        private final JTextField module=field("uwvm.module","0",3),memory=field("uwvm.memory","0",3),
            offset=field("uwvm.offset","0",12),count=field("uwvm.count","256",4);
        private final JLabel state=new JLabel("Read-only allocation tags; pause the selected Wasm session.");
        private final DefaultTableModel rows=new DefaultTableModel(new Object[]{"Byte offset","Allocation tag","Bytes","Palette"},0){
            @Override public boolean isCellEditable(int row,int column){return false;}};
        private final JTable table=new JTable(rows);
        private String next;
        MemoryPanel(Project project) {
            super(new BorderLayout());this.project=project;setName("uwvm.memoryTags");state.setName("uwvm.tagStatus");
            var controls=new JPanel();controls.add(new JLabel("Module"));controls.add(module);controls.add(new JLabel("Memory"));controls.add(memory);
            controls.add(new JLabel("Offset"));controls.add(offset);controls.add(new JLabel("Bytes/page"));controls.add(count);
            var read=new JButton("Read tags");read.setName("uwvm.readTags");read.addActionListener(event->read());controls.add(read);
            var following=new JButton("Next page");following.addActionListener(event->{if(next!=null){offset.setText(next.substring(next.lastIndexOf(':')+1));read();}});controls.add(following);
            add(controls,BorderLayout.NORTH);add(new JScrollPane(table),BorderLayout.CENTER);add(state,BorderLayout.SOUTH);
            table.setName("uwvm.tagTable");
            table.getColumnModel().getColumn(1).setCellRenderer(new DefaultTableCellRenderer(){
                @Override public Component getTableCellRendererComponent(JTable t,Object value,boolean selected,boolean focus,int row,int col) {
                    super.getTableCellRendererComponent(t,value,selected,focus,row,col);
                    Color color=Color.decode(rows.getValueAt(row,3).toString());setBackground(color);
                    setForeground((color.getRed()*299+color.getGreen()*587+color.getBlue()*114)>128000?Color.BLACK:Color.WHITE);
                    setOpaque(true);return this;
                }});
        }
        private void read() {
            rows.setRowCount(0);next=null;
            try {
                var first=unsigned(offset.getText(),64);var m=unsigned(module.getText(),64);var mem=unsigned(memory.getText(),32);
                int bytes=unsigned(count.getText(),16).intValueExact();
                if(bytes>256 || first.add(BigInteger.valueOf(bytes)).bitLength()>64)throw new IllegalArgumentException("Choose a representable 0..256-byte page");
                String reference="wasm-memory:"+m+":"+mem+":"+first;
                request(project,"uwvm/memoryTags",Map.of("memoryReference",reference,"count",bytes,"includeBytes",true),state,
                    response->render(response,reference,first,bytes));
            }catch(RuntimeException error){state.setText(error.getMessage());}
        }
        private void render(JsonObject response,String reference,BigInteger offset,int count) {
            if(!reference.equals(response.get("memoryReference").getAsString()) || count!=response.get("count").getAsInt())
                throw new IllegalArgumentException("Memory selector changed");
            String status=response.get("status").getAsString(),stop=response.get("stopId").getAsString();
            if(!Set.of("available","disabled","unsupported-memory").contains(status) || !stop.matches("[1-9][0-9]*"))
                throw new IllegalArgumentException("Invalid memory status");
            byte[] data=Base64.getDecoder().decode(response.get("data").getAsString());
            if(data.length!=count)throw new IllegalArgumentException("Incomplete memory bytes");
            var tags=response.getAsJsonArray("tags");
            int expected=count==0?0:offset.add(BigInteger.valueOf(count-1)).divide(SIXTEEN).subtract(offset.divide(SIXTEEN)).intValueExact()+1;
            if(tags.size()!=("available".equals(status)?expected:0))throw new IllegalArgumentException("Incomplete tag coverage");
            var prepared=new ArrayList<Object[]>();
            for(int i=0;i<tags.size();i++){
                var row=tags.get(i).getAsJsonObject();var granule=offset.divide(SIXTEEN).add(BigInteger.valueOf(i)).multiply(SIXTEEN);
                int tag=row.get("tag").getAsInt();String hex=row.get("hex").getAsString(),color=row.get("color").getAsString();
                if(!granule.toString().equals(row.get("offset").getAsString()) || tag<0 || tag>255 ||
                    !String.format("0x%02x",tag).equals(hex) || !color.matches("#[0-9a-f]{6}"))
                    throw new IllegalArgumentException("Invalid tag row");
                int begin=granule.max(offset).subtract(offset).intValueExact(),end=granule.add(SIXTEEN).min(offset.add(BigInteger.valueOf(count))).subtract(offset).intValueExact();
                StringBuilder bytes=new StringBuilder();for(int j=begin;j<end;j++)bytes.append(String.format("%02x ",data[j]&255));
                prepared.add(new Object[]{offset.add(BigInteger.valueOf(begin)).toString(),hex,bytes.toString().trim(),color});
            }
            rows.setRowCount(0);prepared.forEach(rows::addRow);
            next=response.has("nextMemoryReference")&&!response.get("nextMemoryReference").isJsonNull()?response.get("nextMemoryReference").getAsString():null;
            state.setText("Snapshot stop "+stop+" | "+status+("available".equals(status)?"":" | No tags synthesized"));
        }
    }
    public static final class HostPanel extends JPanel {
        private final JLabel state=new JLabel("Retained observations only; recording must be explicitly enabled.");
        private final JTextArea text=new JTextArea();
        private final JComboBox<String> family=new JComboBox<>(new String[]{"all","wasip1","ros1","ros2"});
        private final JTextField after=field("uwvm.hostAfter","0",10),call=field("uwvm.call","1",8),segment=field("uwvm.segment","0",4),offset=field("uwvm.byteOffset","0",10);
        HostPanel(Project project) {
            super(new BorderLayout());text.setEditable(false);text.setName("uwvm.hostJSON");var controls=new JPanel();controls.add(family);
            controls.add(new JLabel("After"));controls.add(after);
            var read=new JButton("Read calls");read.setName("uwvm.readHostCalls");read.addActionListener(event->{
                request(project,"uwvm/hostTrace",Map.of("family",family.getSelectedItem(),"after",after.getText(),"count",8),state,r->{
                    text.setText(JSON.toJson(r));after.setText(r.get("next").getAsString());state.setText("Snapshot stop "+r.get("stopId").getAsString());});});controls.add(read);
            controls.add(new JLabel("Call"));controls.add(call);controls.add(new JLabel("Segment"));controls.add(segment);controls.add(new JLabel("Offset"));controls.add(offset);
            var bytes=new JButton("Read bytes");bytes.addActionListener(event->request(project,"uwvm/hostTraceBytes",
                Map.of("call",call.getText(),"segment",segment.getText(),"offset",offset.getText(),"count",4096),state,r->{
                    text.setText(JSON.toJson(r));offset.setText(r.get("next").getAsString());state.setText("Snapshot stop "+r.get("stopId").getAsString());}));controls.add(bytes);
            var export=new JButton("Export JSON");export.addActionListener(event->{
                String path=JOptionPane.showInputDialog(this,"Absolute export path on the debug host (remote host for SSH)");
                if(path!=null)request(project,"uwvm/exportHostTrace",Map.of("path",path,"family",family.getSelectedItem()),state,r->{text.setText(JSON.toJson(r));state.setText("Host trace exported");});
            });controls.add(export);add(controls,BorderLayout.NORTH);add(new JScrollPane(text),BorderLayout.CENTER);add(state,BorderLayout.SOUTH);
        }
    }
}
