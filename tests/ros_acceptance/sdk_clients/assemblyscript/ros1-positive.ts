/** Real ROS1 application entry; pub/sub grants are supplied by the supervisor. */
import { NativeApi } from '../../../../sdk/assemblyscript/ros1/roswasm';
import { run } from './delivery';
export { abortGuest } from '../../../../sdk/assemblyscript/api';
export function _start():void {run(new NativeApi(),false);}
