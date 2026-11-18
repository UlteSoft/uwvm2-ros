package org.uwvm.debug;

import java.awt.BorderLayout;
import java.util.Map;
import java.util.concurrent.CompletableFuture;
import javax.swing.*;

/** Page immutable metadata from one selected VM session and cooperative stop. */
final class UwvmScenePanel extends JPanel {
    record Page(String selection, String stop, int start, int total, int next,
                boolean more, int rows, String json) {}
    interface Source {
        Object session();
        CompletableFuture<Page> read(Object session, Map<String, Object> args);
    }

    private final Source source;
    final JComboBox<String> selection = new JComboBox<>(new String[]{"authority", "resources", "policy", "audit"});
    final JButton read = new JButton("Read scene"), next = new JButton("Next page");
    final JTextArea snapshot = new JTextArea();
    final JLabel status = new JLabel();
    private long sequence;
    private Object owner;
    private Page page;

    UwvmScenePanel(Source source) {
        super(new BorderLayout());
        this.source = source;
        setName("uwvm.sceneState");
        snapshot.setEditable(false);
        var controls = new JPanel();
        controls.add(selection);
        controls.add(read);
        controls.add(next);
        add(controls, BorderLayout.NORTH);
        add(new JScrollPane(snapshot), BorderLayout.CENTER);
        add(status, BorderLayout.SOUTH);
        selection.addActionListener(event -> clear("Select Read scene."));
        read.addActionListener(event -> load(false));
        next.addActionListener(event -> load(true));
        clear("Pause the selected UWVM session and read its scene.");
    }

    private void clear(String message) {
        sequence++;
        owner = null;
        page = null;
        next.setEnabled(false);
        snapshot.setText("");
        status.setText(message);
    }

    private void load(boolean continuation) {
        var previous = page;
        var previousOwner = owner;
        String selected = (String) selection.getSelectedItem();
        clear("Reading " + selected + "...");
        long ticket = sequence;
        try {
            var session = source.session();
            if (continuation && (previous == null || !previous.more() || session != previousOwner))
                throw new IllegalStateException("Select Read scene for the current session");
            int start = continuation ? previous.next() : 0;
            Map<String, Object> args = continuation
                ? Map.of("selection", selected, "count", 8, "start", start, "ifStop", previous.stop())
                : Map.of("selection", selected, "count", 8, "start", start);
            source.read(session, args).whenComplete((result, error) -> SwingUtilities.invokeLater(() -> {
                if (sequence != ticket) return;
                try {
                    if (error != null) throw new IllegalStateException(error.getMessage());
                    if (session != source.session() || !selected.equals(result.selection()) ||
                        result.start() != start || result.stop() == null ||
                        !result.stop().matches("[1-9][0-9]{0,19}") ||
                        result.stop().length() == 20 && result.stop().compareTo("18446744073709551615") > 0 ||
                        continuation && !previous.stop().equals(result.stop()))
                        throw new IllegalStateException("Scene session, selector or stop changed");
                    int rows = result.rows(), total = result.total();
                    if (total < 0 || total > 4096 || rows != Math.min(8, Math.max(0, total - start)) ||
                        result.next() != (rows > 0 ? start + rows : 0) || result.more() != (start + rows < total))
                        throw new IllegalStateException("Incomplete scene page");
                    page = result;
                    owner = session;
                    next.setEnabled(result.more());
                    snapshot.setText(result.json());
                    snapshot.setCaretPosition(0);
                    status.setText("Snapshot at stop " + result.stop() + " | " + total + " rows");
                } catch (RuntimeException invalid) {
                    clear("Unavailable: " + invalid.getMessage());
                }
            }));
        } catch (RuntimeException error) {
            clear("Unavailable: " + error.getMessage());
        }
    }
}
