/** Actual ROS1 WASI command. Standard input contains one explicit counted alias.
 * Default-zero Runtime authority exits13 before subscription or a native effect.
 */
import {readOnce,cleanupStatus} from "./telemetry_melodic_myarm";
import {readAlias,proc_exit} from "./wasi_alias";
export function _start():void {const input=readAlias();const code=input.status?input.status:readOnce(input.alias);const finalCode=code?code:cleanupStatus;proc_exit(finalCode<126?finalCode:125);}
