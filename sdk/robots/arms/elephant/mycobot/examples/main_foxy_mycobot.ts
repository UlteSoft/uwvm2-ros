/** Actual ROS2 WASI command; this module imports only the selected backend.
 * Read one counted alias from basic stdin; no implicit grant or retry exists.
 */
import {readOnce,cleanupStatus} from "./telemetry_foxy_mycobot";
import {readAlias,proc_exit} from "./wasi_alias";
export function _start():void {const input=readAlias();const code=input.status?input.status:readOnce(input.alias);const finalCode=code?code:cleanupStatus;proc_exit(finalCode<126?finalCode:125);}
