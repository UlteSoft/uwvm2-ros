/** Actual ROS2 WASI command; this module imports only the selected backend.
 * Read one counted alias from basic stdin; no implicit grant or retry exists.
 */
import {readOnce} from "./telemetry_ros2";
import {readAlias,proc_exit} from "./wasi_alias";
export function _start():void {const input=readAlias();const code=input.status?input.status:readOnce(input.alias);proc_exit(code<126?code:125);}
