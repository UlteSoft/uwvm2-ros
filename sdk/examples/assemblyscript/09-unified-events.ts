/** Runnable ROS2 entry with real imports and checked owner cleanup.
 * Explicit grants and native prerequisites: sdk/docs/unified-events.md.
 */
import {NativeApi} from '../../assemblyscript/ros2/roswasm';
import {run} from './unified-events';
export {abortGuest} from '../../assemblyscript/api';
export {lastStatus} from './unified-events';
/** Execute on the caller thread; no implicit native thread, clock or retry. */
export function _start():void {run(new NativeApi());}
