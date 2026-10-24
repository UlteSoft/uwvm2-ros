/** Real ROS2 default-denial application; constructors grant no authority. */
import { NativeApi } from '../../../../sdk/origin/assemblyscript/ros2/roswasm';
import { run } from './delivery';
export { abortGuest } from '../../../../sdk/origin/assemblyscript/api';
export function _start():void {run(new NativeApi(),true);}
