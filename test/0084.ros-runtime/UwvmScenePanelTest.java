package org.uwvm.debug;

import java.util.Map;
import java.util.concurrent.CompletableFuture;
import javax.swing.SwingUtilities;

/** Execute the JetBrains scene panel's actual Swing handlers on the EDT. */
public final class UwvmScenePanelTest {
    private static final String STOP = "18446744073709551615";
    private static final class Source implements UwvmScenePanel.Source {
        Object owner = new Object();
        Map<String, Object> last;
        CompletableFuture<UwvmScenePanel.Page> reply;
        public Object session() { return owner; }
        public CompletableFuture<UwvmScenePanel.Page> read(Object session, Map<String, Object> args) {
            assert session == owner;
            last = Map.copyOf(args);
            reply = new CompletableFuture<>();
            return reply;
        }
        void finish(String selection, String stop, int start, int total) {
            int rows = Math.min(8, total - start);
            reply.complete(new UwvmScenePanel.Page(selection, stop, start, total, start + rows,
                start + rows < total, rows, selection + "/" + stop + "/" + start));
        }
    }

    public static void main(String[] args) throws Exception {
        var source = new Source();
        var panel = new UwvmScenePanel[1];
        SwingUtilities.invokeAndWait(() -> panel[0] = new UwvmScenePanel(source));
        var view = panel[0];
        for (String selected : new String[]{"authority", "resources", "policy", "audit"}) {
            SwingUtilities.invokeAndWait(() -> {
                view.selection.setSelectedItem(selected);
                view.read.doClick();
                assert source.last.equals(Map.of("selection", selected, "count", 8, "start", 0));
                source.finish(selected, STOP, 0, selected.equals("resources") ? 9 : 1);
            });
            SwingUtilities.invokeAndWait(() -> {
                assert !view.snapshot.isEditable() && view.snapshot.getText().equals(selected + "/" + STOP + "/0");
                assert view.next.isEnabled() == selected.equals("resources");
                if (view.next.isEnabled()) {
                    view.next.doClick();
                    assert source.last.equals(Map.of("selection", selected, "count", 8, "start", 8, "ifStop", STOP));
                    source.finish(selected, STOP, 8, 9);
                }
            });
            if (selected.equals("resources")) SwingUtilities.invokeAndWait(() -> {
                assert !view.next.isEnabled() && view.snapshot.getText().endsWith("/8");
            });
        }
        SwingUtilities.invokeAndWait(() -> {
            view.read.doClick();
            source.reply.completeExceptionally(new IllegalStateException("Permission denied"));
        });
        SwingUtilities.invokeAndWait(() -> {
            assert view.snapshot.getText().isEmpty() && view.status.getText().contains("Permission denied");
            view.read.doClick();
            var old = source.reply;
            view.selection.setSelectedItem("resources");
            old.complete(new UwvmScenePanel.Page("audit", STOP, 0, 0, 0, false, 0, "retired"));
        });
        SwingUtilities.invokeAndWait(() -> {
            assert view.snapshot.getText().isEmpty();
            view.read.doClick();
            source.finish("resources", STOP, 0, 9);
        });
        SwingUtilities.invokeAndWait(() -> {
            assert view.next.isEnabled();
            source.owner = new Object();
            var before = source.reply;
            view.next.doClick();
            assert source.reply == before && view.snapshot.getText().isEmpty();
            view.read.doClick();
            source.finish("resources", STOP, 0, 9);
        });
        SwingUtilities.invokeAndWait(() -> {
            view.next.doClick();
            source.finish("resources", "1", 8, 9);
        });
        SwingUtilities.invokeAndWait(() -> {
            assert view.snapshot.getText().isEmpty() && !view.next.isEnabled() && view.status.getText().contains("stop changed");
            view.read.doClick();
            source.finish("resources", "18446744073709551616", 0, 1);
        });
        SwingUtilities.invokeAndWait(() -> {
            assert view.snapshot.getText().isEmpty() && !view.next.isEnabled();
        });
        System.out.println("PASS Swing scene selectors, exact stop, continuation, denied, retired and replaced sessions");
    }
}
