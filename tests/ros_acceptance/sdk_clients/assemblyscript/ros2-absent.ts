/** Real ROS2 default-denial application; constructors grant no authority. */
import { NativeApi } from '../../../../sdk/assemblyscript/ros2/roswasm';
import { run } from './delivery';
export { abortGuest } from '../../../../sdk/assemblyscript/api';
export function _start():void {run(new NativeApi(),true);}
