"""Owned native witness using the EXACT original rospy echo implementation.

Only Root's selected Noetic/Focal Python executes this program. The fixture is
the Git-verified original listenerpublisher.py, SHA-bound again by the port.
Its start_publishing/callback functions are executed without replacing either
ROS operation. This wrapper adds finite control gates and observations; it does
not start a Master or manufacture native receipt bytes on the coordinator.
All upstream print output goes to stderr, separate from this bounded protocol.
"""
import argparse
import importlib.util
import json
import os
import select
import sys
import time


def need(value,reason):
    if not value: raise ValueError(reason)


def main():
    parser=argparse.ArgumentParser(allow_abbrev=False)
    parser.add_argument('--fixture',required=True)
    parser.add_argument('--mode',choices=('positive','default-denied'),required=True)
    args=parser.parse_args()
    need(os.environ.get('ROS_IP')=='127.0.0.1' and 'ROS_HOSTNAME' not in os.environ and
         'ROS_NAMESPACE' not in os.environ and 'LD_PRELOAD' not in os.environ, 'literal isolated ROS1 environment required')
    protocol=sys.stdout
    sys.stdout=sys.stderr
    def emit(value):
        raw=json.dumps(value,sort_keys=True,separators=(',',':'),allow_nan=False)+'\n'
        need(len(raw)<=4096,'bounded native protocol record required');protocol.write(raw);protocol.flush()
    def control():
        ready,_,_=select.select([sys.stdin.buffer],[],[],30)
        need(ready,'native protocol deadline');value=os.read(sys.stdin.fileno(),1)
        need(len(value)==1,'complete native protocol token required');return value
    import rospy
    from std_msgs.msg import String
    spec=importlib.util.spec_from_file_location('original_official_listenerpublisher',args.fixture)
    module=importlib.util.module_from_spec(spec);spec.loader.exec_module(module)
    emit({'event':'native-gate','pid':os.getpid()})
    need(control()==b'G','native context gate required')
    rospy.init_node('listenerpublisher',disable_signals=True)
    seen=[];fault=[]
    def callback(message):
        try:
            need(type(message.data) is str and message.data=='hi [42]' and len(seen)<10,
                 'native original callback observed a foreign/excess payload')
            # Original upstream code creates the echo publisher on first data
            # and publishes String(data.data); never substitute a fake echo.
            module.callback(message);seen.append(message.data)
        except BaseException as error: fault.append(repr(error))
    subscriber=rospy.Subscriber('chatter',String,callback)
    need(subscriber.get_num_connections()==0,'native subscriber must exist BEFORE any chatter publisher')
    emit({'event':'native-subscribed','publishers':0})
    while True:
        value=control();need(not fault,'native callback failed: '+repr(fault))
        if value==b'M':
            until=time.monotonic()+5
            while subscriber.get_num_connections()!=1 and time.monotonic()<until and not rospy.is_shutdown():time.sleep(.01)
            need(subscriber.get_num_connections()==1,'one actual Gateway chatter publisher required')
            emit({'event':'native-matched','publishers':1})
        elif value==b'E':
            until=time.monotonic()+5
            while len(seen)<10 and time.monotonic()<until and not fault:time.sleep(.01)
            need(args.mode=='positive' and not fault and len(seen)==10 and all(v=='hi [42]' for v in seen),
                 'all ten native original echo callbacks required')
            emit({'message':seen[-1]})
        elif value==b'Q':
            need(args.mode=='default-denied' and not seen,'denied native witness already observed an effect')
            start=time.monotonic_ns();samples=0
            while time.monotonic_ns()-start<2000000000 or samples<30:
                need(not seen and not fault,'denied native witness observed a ROS effect');samples+=1;time.sleep(.01)
            emit({'effects':len(seen),'samples':samples,'elapsed_ns':time.monotonic_ns()-start})
        elif value==b'F':break
        else:raise ValueError('unexpected native protocol token')
    subscriber.unregister()
    if module._pub is not None:module._pub.unregister()
    rospy.signal_shutdown('owned official port complete')


if __name__=='__main__':main()
