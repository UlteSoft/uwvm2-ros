"""Noetic rospy String publisher or subscriber for the upstream bridge test."""
import sys
import time

import rospy
from std_msgs.msg import String


def main():
    role, token, seconds = sys.argv[1:]
    seconds = int(seconds)
    if role not in ('send', 'receive') or len(token) != 32 or not 1 <= seconds <= 15:
        return 1
    rospy.init_node('wros_ros1_witness', disable_rosout=True)
    observations = set()
    if role == 'send':
        endpoint = rospy.Publisher('chatter', String, queue_size=4)
    else:
        endpoint = rospy.Subscriber('chatter', String, lambda msg: observations.add(msg.data), queue_size=4)
    stop = time.monotonic() + seconds
    sent = 0
    while not observations and time.monotonic() < stop:
        if role == 'send':
            endpoint.publish(String(data=token))
            sent += 1
        time.sleep(0.05)
    endpoint.unregister()
    expected = token in observations
    wrong = bool(observations.difference([token]))
    print('role=%s sent=%d received=%d unexpected=%d value=%s' %
          (role, sent, expected, wrong, token if expected or sent else ''))
    if wrong:
        return 4
    return 0 if expected or sent else 3


if __name__ == '__main__':
    sys.exit(main())
