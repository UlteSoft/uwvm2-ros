"""Own actual fixture processes through physical retirement; no Guest/backend shim."""
import ctypes,os,signal,subprocess,tempfile,time
from pathlib import Path

def process_identity(pid):
    try:
        text = (Path("/proc") / str(pid) / "stat").read_text()
        fields = text[text.rfind(")") + 2:].split()
        return {"pid": pid, "state": fields[0], "process_group": int(fields[2]),
            "session": int(fields[3]), "start_ticks": int(fields[19])}
    except (OSError, ValueError):
        return None

def group_members(group):
    if not Path("/proc").is_dir():
        return []
    result = []
    for entry in Path("/proc").iterdir():
        if entry.name.isdigit():
            identity = process_identity(int(entry.name))
            if identity and identity["process_group"] == group:
                result.append(identity)
    return result

def run_owned(command, timeout=45):
    # WNOWAIT keeps the original leader unreaped until its group is empty. Its
    # PID/PGID/SID therefore cannot be recycled while any group kill is possible.
    if not hasattr(os, "WNOWAIT") or not Path("/proc").is_dir():
        raise RuntimeError("Thread lifecycle tests require Linux waitid/procfs")
    if signal.getsignal(signal.SIGCHLD) != signal.SIG_DFL:
        raise RuntimeError("Owned WNOWAIT cleanup requires the default SIGCHLD disposition")
    native = ctypes.CDLL(None, use_errno=True)
    if native.prctl(36, 1, 0, 0, 0) != 0:  # PR_SET_CHILD_SUBREAPER
        raise OSError(ctypes.get_errno(), "Cannot own orphaned launcher children")
    with tempfile.TemporaryFile() as out, tempfile.TemporaryFile() as err:
        creation_mask = signal.pthread_sigmask(signal.SIG_BLOCK, {signal.SIGTERM, signal.SIGINT})
        try:
            process = subprocess.Popen(command, stdout=out, stderr=err, start_new_session=True,
                preexec_fn=lambda: signal.pthread_sigmask(signal.SIG_SETMASK, creation_mask))
        except BaseException:
            signal.pthread_sigmask(signal.SIG_SETMASK, creation_mask)
            raise
        leader = process_identity(process.pid)
        if not leader or leader["process_group"] != process.pid or leader["session"] != process.pid:
            # No group operation is permitted without the original identity.
            try:
                process.wait(timeout=5)
            finally:
                signal.pthread_sigmask(signal.SIG_SETMASK, creation_mask)
            raise RuntimeError("Cannot authenticate the original process session")
        keys = ("pid", "process_group", "session", "start_ticks")
        def original_leader():
            actual = process_identity(process.pid)
            return actual and all(actual[key] == leader[key] for key in keys)
        def members():
            if not original_leader():
                raise RuntimeError("Original leader identity lost before group cleanup")
            rows = group_members(process.pid)
            if any(row["session"] != leader["session"] or row["start_ticks"] < leader["start_ticks"] for row in rows):
                raise RuntimeError("Refusing to signal an unauthenticated process group")
            return rows
        def exited():
            return os.waitid(os.P_PID, process.pid, os.WEXITED | os.WNOHANG | os.WNOWAIT) is not None
        timed_out, interrupted_error = False, None
        try:
            signal.pthread_sigmask(signal.SIG_SETMASK, creation_mask)
            deadline = time.monotonic() + timeout
            while not exited():
                if time.monotonic() >= deadline:
                    timed_out = True
                    break
                time.sleep(0.005)
        except BaseException as error:
            interrupted_error = error
        # Never use communicate(), poll(), wait() or group-wide waitpid before
        # this cleanup: each could reap the leader and release its identity.
        previous_mask = signal.pthread_sigmask(signal.SIG_BLOCK, {signal.SIGTERM, signal.SIGINT})
        try:
            before = members()
            remaining = [row for row in before if row["pid"] != process.pid]
            signalled = timed_out or interrupted_error is not None or bool(remaining)
            if signalled:
                if not original_leader():
                    raise RuntimeError("Leader changed immediately before group signal")
                os.killpg(process.pid, signal.SIGKILL)
            deadline = time.monotonic() + 5
            while True:
                others = [row for row in members() if row["pid"] != process.pid]
                for row in others:
                    try:
                        os.waitpid(row["pid"], os.WNOHANG)
                    except ChildProcessError:
                        pass
                if exited() and not [row for row in members() if row["pid"] != process.pid]:
                    break
                if time.monotonic() >= deadline:
                    raise RuntimeError("Original process group did not physically retire")
                time.sleep(0.005)
            before_reap = process_identity(process.pid)
            cleanup = dict(leader=leader, leader_before_reap=before_reap,
                identity_preserved_until_group_cleanup=bool(original_leader()),
                group_signalled=signalled, original_descendants_before_cleanup=remaining,
                original_descendants_before_leader_reap=[])
            code = process.wait(timeout=5)  # Only now may the leader be reaped.
            cleanup.update(leader_reaped=True, complete=True)
            # No numeric PGID lookup or signal is performed after that reap.
            out.seek(0)
            err.seek(0)
            stdout, stderr = out.read(), err.read()
            if interrupted_error is not None:
                interrupted_error.cleanup = cleanup
                raise interrupted_error
            return code, stdout, stderr, timed_out, leader, remaining, cleanup
        finally:
            signal.pthread_sigmask(signal.SIG_SETMASK, previous_mask)

def interrupted(signum, _frame):
    raise KeyboardInterrupt("Thread test interrupted by signal " + str(signum))
