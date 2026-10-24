/** Real ROS2 application entry; exact native type/hash/QoS come from policy. */
import { NativeApi } from '../../../../sdk/origin/assemblyscript/ros2/roswasm';
import { run } from './delivery';
export { abortGuest } from '../../../../sdk/origin/assemblyscript/api';
export function _start():void {run(new NativeApi(),false);}
