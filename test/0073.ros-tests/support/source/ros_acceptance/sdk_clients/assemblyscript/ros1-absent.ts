/** Real ROS1 default-denial application; no ROS alias is granted. */
import { NativeApi } from '../../../../sdk/origin/assemblyscript/ros1/roswasm';
import { run } from './delivery';
export { abortGuest } from '../../../../sdk/origin/assemblyscript/api';
export function _start():void {run(new NativeApi(),true);}
