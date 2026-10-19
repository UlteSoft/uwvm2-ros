/** Real ROS1 default-denial application; no ROS alias is granted. */
import { NativeApi } from '../../../../sdk/assemblyscript/ros1/roswasm';
import { run } from './delivery';
export { abortGuest } from '../../../../sdk/assemblyscript/api';
export function _start():void {run(new NativeApi(),true);}
