"""Explicit finite native Isaac Sim JointState graph, never a hardware driver.

The actual original node IDs and field links are selected from fixed87c95274.
All Isaac/Kit imports happen after explicit admission. Normal Sim update/model
loading can block: the operator must retain a finite whole-process owner. Local
steps/time caps do not claim preemptive GPU cancellation or physical stop.
"""
import argparse
import os
from pathlib import Path
import time


class Refused(Exception):
    def __init__(self,code):self.code=code


def main(argv=None):
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--enable-native-simulation',action='store_true')
    p.add_argument('--enable-sim-commands',action='store_true')
    p.add_argument('--sdk-root',required=True);p.add_argument('--usd',required=True)
    p.add_argument('--articulation',required=True);p.add_argument('--domain',type=int,required=True)
    p.add_argument('--state-topic',required=True);p.add_argument('--command-topic',required=True)
    p.add_argument('--state-qos-json',required=True);p.add_argument('--command-qos-json',required=True)
    p.add_argument('--frames',type=int,required=True);p.add_argument('--lifetime-ms',type=int,required=True)
    a=p.parse_args(argv)
    if not a.enable_native_simulation:return 13
    if not 0<=a.domain<=232 or not 1<=a.frames<=64 or not 1<=a.lifetime_ms<=10000:return 22
    sdk=Path(a.sdk_root);usd=Path(a.usd)
    if not sdk.is_absolute() or sdk.resolve(strict=True)!=sdk or not sdk.is_dir():return 22
    if not usd.is_absolute() or usd.resolve(strict=True)!=usd or not usd.is_file():return 22
    if not a.articulation.startswith('/') or not a.state_topic.startswith('/') or not a.command_topic.startswith('/'):return 22
    if len(a.state_qos_json)>2048 or len(a.command_qos_json)>2048:return 90
    # Root selected native ROS domain, not Guest authority or an ambient default.
    os.environ['ROS_DOMAIN_ID']=str(a.domain)
    until=time.monotonic_ns()+a.lifetime_ms*1000000
    import isaacsim
    if not Path(isaacsim.__file__).resolve().is_relative_to(sdk):return 13
    from isaacsim import SimulationApp
    simulation=SimulationApp({'headless':True})
    cleanup=[];primary=0;frames=0;app_utils=None
    try:
        import isaacsim.core.experimental.utils.app as app_utils
        import isaacsim.core.experimental.utils.stage as stage_utils
        from isaacsim.core.simulation_manager import SimulationManager
        import omni.graph.core as og
        import usdrt.Sdf
        app_utils.enable_extension('isaacsim.ros2.bridge')
        simulation.update()
        success,stage=stage_utils.open_stage(str(usd))
        if not success or stage is None:raise Refused(5)
        if not stage.GetPrimAtPath(a.articulation).IsValid():raise Refused(22)
        if time.monotonic_ns()>=until:raise Refused(110)
        nodes=[('Tick','omni.graph.action.OnPlaybackTick'),
          ('Read','isaacsim.sensors.physics.IsaacReadJointState'),
          ('Publish','isaacsim.ros2.bridge.ROS2PublishJointState'),
          ('Time','isaacsim.core.nodes.IsaacReadSimulationTime')]
        values=[('Read.inputs:prim',[usdrt.Sdf.Path(a.articulation)]),
          ('Publish.inputs:topicName',a.state_topic),('Publish.inputs:qosProfile',a.state_qos_json)]
        links=[('Tick.outputs:tick','Read.inputs:execIn'),('Read.outputs:execOut','Publish.inputs:execIn'),
          ('Time.outputs:simulationTime','Publish.inputs:timeStamp')]
        for member in ['jointNames','jointPositions','jointVelocities','jointEfforts','jointDofTypes','stageMetersPerUnit','sensorTime']:
            links.append(('Read.outputs:'+member,'Publish.inputs:'+member))
        if a.enable_sim_commands:
            # Only this explicitly enabled branch creates an actual ROS command
            # subscriber. Each real execOut drives the controller, preserving
            # genuine Sim semantics. No fake per-message counter is claimed.
            nodes += [('Subscribe','isaacsim.ros2.bridge.ROS2SubscribeJointState'),
                      ('Control','isaacsim.core.nodes.IsaacArticulationController')]
            values += [('Subscribe.inputs:topicName',a.command_topic),('Subscribe.inputs:qosProfile',a.command_qos_json),
                       ('Control.inputs:robotPath',a.articulation)]
            links += [('Tick.outputs:tick','Subscribe.inputs:execIn'),('Subscribe.outputs:execOut','Control.inputs:execIn')]
            for member in ['jointNames','positionCommand','velocityCommand','effortCommand']:
                links.append(('Subscribe.outputs:'+member,'Control.inputs:'+member))
        og.Controller.edit({'graph_path':'/UWVM_NVIDIA_ExplicitGraph','evaluator_name':'execution'},
          {og.Controller.Keys.CREATE_NODES:nodes,og.Controller.Keys.SET_VALUES:values,og.Controller.Keys.CONNECT:links})
        SimulationManager.setup_simulation(dt=1.0/60.0,device='cpu')
        app_utils.play()
        while simulation.is_running() and frames<a.frames and time.monotonic_ns()<until:
            simulation.update();frames+=1
        if time.monotonic_ns()>=until:primary=110
    except Refused as error:primary=error.code
    except Exception:primary=5
    finally:
        if app_utils is not None:
            try:app_utils.stop();cleanup.append(('timeline',0))
            except Exception as error:cleanup.append(('timeline',error))
        try:simulation.close();cleanup.append(('application',0))
        except Exception as error:cleanup.append(('application',error))
        main.last={'primary':primary,'cleanup':cleanup,'actual_frames':frames,'commands_enabled':a.enable_sim_commands}
    return primary or (5 if any(isinstance(v,Exception) for _,v in cleanup) else 0)


if __name__=='__main__':raise SystemExit(main())
