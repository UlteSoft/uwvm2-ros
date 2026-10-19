package org.uwvm.roswasm.vendor;

import java.util.List;
import org.uwvm.roswasm.*;

/** Real TeaVM linear WASI Guest. SOURCE_ONLY_UNEXECUTED. */
public final class JointStateLesson {
    private JointStateLesson() {}
    public static Throwable lastPrimary;
    public static Throwable lastCleanupException;
    public static List<Result> lastCleanupResults;
    public static List<Throwable> lastCleanupFailures;
    public static Session lastSession;
    public static int receivedCount;
    public static boolean deniedVerified;

    public static void main(String[] args) throws Exception {
        if (args.length != 0 || (LessonMode.VALUE != 0 && LessonMode.VALUE != 1))
            throw new IllegalArgumentException("one fresh command, build-selected mode");
        Selected.checkCodec();
        Session session = new Session(Selected.transport(), 1);
        lastSession = session; // keep the actual owner/diagnostic ledger reachable
        Throwable primary = null;
        try {
            session.checkAbi();
            if (LessonMode.VALUE == 0) {
                try {
                    session.open("joint-state");
                    throw new IllegalStateException("default-denied open unexpectedly succeeded");
                } catch (RosException denied) {
                    if (denied.result.error != 13 || denied.result.value != null) throw denied;
                    deniedVerified = true;
                }
            } else {
                Node.Pollable subscription = Selected.subscribe(new Node(session));
                for (int credit = 0; credit < 64 && receivedCount == 0; ++credit) {
                    if (System.in.read() != 'R')
                        throw new IllegalArgumentException("one actual binary R byte per poll");
                    Result result = subscription.pollOnce();
                    if (result.error != 0 && result.error != 11) result.checked();
                }
                if (receivedCount != 1)
                    throw new IllegalStateException("no one complete value within64 credits");
            }
        } catch (Exception | Error failure) {
            primary = lastPrimary = failure;
        } finally {
            try { session.close(); }
            catch (RuntimeException | Error cleanup) {
                lastCleanupException = cleanup;
                if (primary == null) primary = cleanup;
                else try { primary.addSuppressed(cleanup); }
                     catch (RuntimeException | Error unavailable) { /* Independent field retained. */ }
            } finally {
                lastCleanupResults = session.lastCleanupResults();
                lastCleanupFailures = session.lastCleanupFailures();
            }
        }
        if (primary instanceof Error) throw (Error)primary;
        if (primary != null) throw (Exception)primary;
        if (!session.pendingRetirements().isEmpty())
            throw new IllegalStateException("known unattempted owner remains; no replay");
        if (LessonMode.VALUE == 0 && (!deniedVerified || receivedCount != 0))
            throw new IllegalStateException("default denial oracle");
        // Actual cleanup has returned before the sole success marker.
        System.out.println(LessonMode.VALUE == 0 ?
            "PASS_JOINT_STATE_DEFAULT_DENIED" : "PASS_JOINT_STATE_FULL_VALUE");
        System.out.flush();
    }
}
