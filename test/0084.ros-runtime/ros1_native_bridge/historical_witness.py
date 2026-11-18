"""A Python 2 ROS 1 endpoint for official historical release packages."""
from __future__ import print_function
import inspect
import json
import os
import sys
import time

import genpy
import rosgraph
import rospy
from std_msgs.msg import String


def main():
    role, token, seconds = sys.argv[1:]
    expected = os.path.realpath(os.environ['WROS_ROS1_PYTHON_PREFIX']) + '/'
    files = {module.__name__: os.path.realpath(module.__file__)
             for module in (rospy, genpy, rosgraph, sys.modules[String.__module__])}
    if (sys.version_info[:2] != (2,7) or any(not p.startswith(expected) for p in files.values())
            or String._md5sum != '992ce8a1687cec8c8bd883ec73ca41d1'):
        return 1
    print(json.dumps({'python':sys.version.split()[0], 'distro':os.environ['ROS_DISTRO'],
                      'imports':files,'stringMD5':String._md5sum},sort_keys=True))
    if role not in ('send','receive') or len(token)!=32 or not 1<=int(seconds)<=15:
        return 1
    rospy.init_node('wros_ros1_witness',disable_rosout=True)
    seen=set()
    if role=='send':
        options={'queue_size':4} if 'queue_size' in inspect.getargspec(rospy.Publisher.__init__).args else {}
        endpoint=rospy.Publisher('chatter',String,**options)
    else:
        endpoint=rospy.Subscriber('chatter',String,lambda msg:seen.add(msg.data),queue_size=4)
    sent=0
    for _ in range(int(seconds)*20):
        if seen:
            break
        if role=='send':
            endpoint.publish(String(data=token))
            sent+=1
        time.sleep(.05)
    endpoint.unregister()
    expected=token in seen
    wrong=bool(seen.difference([token]))
    print('role=%s sent=%d received=%d unexpected=%d value=%s'%
          (role,sent,expected,wrong,token if expected or sent else ''))
    return 4 if wrong else (0 if expected or sent else 3)


if __name__=='__main__':
    sys.exit(main())
