/** Runnable ROS 1 entry; see shared lesson and unified-events.md policy. */
import {NativeApi} from '../../../assemblyscript/ros1/roswasm';
import {run} from '../unified-events';
export {abortGuest} from '../../../assemblyscript/api';
export {lastStatus} from '../unified-events';
/** Execute complete imports/cleanup on the guest thread, without native reentry. */
export function _start():void {run(new NativeApi());}
