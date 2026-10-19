"""Actual serial Linux short-child stress entry for an already admitted Frame.

There is no standalone bootstrap, candidate import, environment/tool discovery
or action on import. Root supplies its exact new full freeze and its original
current64/swap0 Frame, owns the outer deadline/RSS and final source/root checks,
and explicitly calls stress(common, frame). Here common is the current
source-bound tests/ros_unitree/common.py module; frame.common is its separately
source-bound tools/ros_distro/common.py dependency. No ROS/Guest is executed.
"""
from pathlib import Path
import os
import sys


def stress(common, frame, count=512, *, python_input='python'):
    """Run count REAL held-pidfd children, one at a time, then retain raw proof.

    Each child blocks for one stdin byte before exiting. This establishes a
    real original held pidfd BEFORE release, then maximizes ordinary short-exit
    interleavings around Actors.check identity -> /proc/PID/cgroup. A stress
    PASS does not claim every race occurred; deterministic DATA tests exercise
    each exact interleaving separately. No numeric PID or process-group signal
    is sent. Root must still call Frame.finish on success AND on exception.
    """
    common.require(type(count) is int and 16 <= count <= 4096 and sys.platform == 'linux',
                   'explicit bounded actual Linux stress count required')
    frame.guard(); cgroup = frame.config['cgroup']
    common.require(cgroup['memory_max'] == 68719476736 and cgroup['memory_swap_max'] == 0 and
                   frame.limits['jobs'] == 1 and frame.limits['rss_bytes'] <= 768*1024**2,
                   'original current64/swap0 serial bounded parent/child owner required')
    frozen, manifest_pin = frame.common.document(frame.manifest)
    common.require(frame.pin(frame.manifest) == manifest_pin and
                   manifest_pin['sha256'] == frame.report['source_manifest_sha256'],
                   'actual already-admitted Frame manifest changed')
    for module, relative in ((common,'tests/ros_unitree/common.py'),
                             (frame.common,'tools/ros_distro/common.py')):
        actual = frame.pin(frame.source/relative)
        common.require(Path(module.__file__).resolve(strict=True) == Path(actual['path']) and
                       frozen.get(relative) == {name:actual[name] for name in ('bytes','sha256')},
                       'actual current complete-MF helper role required')
    own = frame.pin(Path(__file__).resolve(strict=True))
    relative = 'tests/ros_unitree/actor_exit_stress.py'
    common.require(own['path'] == str(frame.source/relative) and frozen.get(relative) ==
                   {name:own[name] for name in ('bytes','sha256')}, 'actual frozen stress entry required')
    selected = frame.inputs[python_input]; executable = frame.input(python_input)
    common.require(frame.pin(executable) == selected and executable == Path(sys.executable).resolve(strict=True) and
                   os.access(executable,os.X_OK), 'actual explicitly admitted current native Python required')
    frame.common.cgroup_preserved(frame.before,frame.common.cgroup_state(cgroup))
    marker = b'UWVM_SHORT_LIVED_PIDFD\n'
    code = 'import os; b=os.read(0,1); assert b==b"X"; os.write(1,b"UWVM_SHORT_LIVED_PIDFD\\n")'
    report = {'kind':'actual-owned-short-process-procfs-exit-stress','status':'RUNNING',
        'requested_actual_children':count,'completed_actual_children':0,'children':[],
        'Runtime_executed':False,'ROS_executed':False,'Guest_rights_granted':[],
        'scope':'REAL_LINUX_PIDFD_CHILD_EXITS_NOT_ROS_OR_WASM_QUALIFICATION',
        'deterministic_races_claimed_observed':False}
    frame.report['actor_exit_stress'] = report; frame.save()
    try:
        for index in range(count):
            frame.guard(); owner = common.Actors(frame)
            try:
                child,row = owner.start('short-%04d'%index,[str(executable),'-I','-B','-c',code],json_output=False)
                original = row['first_identity']
                entries = [(key,value) for key,value in owner.owner.owned.items() if key[0] == child.pid]
                common.require(original is not None and len(entries) == 1 and
                    entries[0][0] == frame.common.birth(original) and
                    not frame.common.pidfd_terminal(entries[0][1]['fd']), 'real original live pidfd before release required')
                # After this write the short child exits immediately. Every
                # finish/check still uses the exact original retained pidfd.
                owner.write(child,b'X'); completed = owner.finish(child)
                raw,pin = frame.common.body_pin(Path(completed['stdout']),65536)
                common.require(raw == marker and completed['returncode'] == 0,
                               'real short child output/zero-exit differs')
                record = {'ordinal':index,'original_birth':original,'stdout':pin,'returncode':0,
                          'live_owned_pidfd_before_release':True}
                report['children'].append(record)
            finally:
                cleanup = owner.close()
                common.require(cleanup['status'] == 'PASS' and not cleanup.get('remaining'), 'short-child cleanup incomplete')
                frame.actors.remove(owner)
            record['cleanup'] = cleanup; report['completed_actual_children'] += 1
            frame.save()
        common.require(report['completed_actual_children'] == count, 'all selected real short children must finish')
        report['status'] = 'PASS_ACTUAL_SERIAL_SHORT_LIVED_PIDFD_EXITS_ONLY'
    except BaseException as error:
        report.update(status='FAIL',error=repr(error)); raise
    finally:
        frame.save()
    return report
