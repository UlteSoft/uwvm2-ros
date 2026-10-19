/** Actual ROS1 WASI command. Standard input contains one explicit counted alias.
 * Default-zero Runtime authority exits13 before subscription or a native effect.
 */
import {readOnce} from "./telemetry_ros1";
import {readAlias,proc_exit} from "./wasi_alias";
export function _start():void {const input=readAlias();const code=input.status?input.status:readOnce(input.alias);proc_exit(code<126?code:125);}
