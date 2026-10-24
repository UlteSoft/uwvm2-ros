/** Real ROS1 application entry; pub/sub grants are supplied by the supervisor. */
import { NativeApi } from '../../../../sdk/origin/assemblyscript/ros1/roswasm';
import { run } from './delivery';
export { abortGuest } from '../../../../sdk/origin/assemblyscript/api';
export function _start():void {run(new NativeApi(),false);}
