/** Actual AssemblyScript module initialization/main, selected by its real void
 * legacy_command_entry export (--exportStart legacy_command_entry). No WAT
 * positive wrapper or preload calls into a ROS-privileged library. The actual
 * AS runtime/global initialization precedes this finite command exactly once.
 */
import {initialize_case,poll_once,submit_once,case_status,received_count,denied_count,close_case,observed_length,observed_byte} from '../../../sdk/robots/unitree/examples/legacy/lowlevel';
export {initialize_case,poll_once,submit_once,case_status,received_count,denied_count,close_case,observed_length,observed_byte} from '../../../sdk/robots/unitree/examples/legacy/lowlevel';
import {setup,control,output,finish} from './protocol';
let opened=false;
function require(ok:bool,code:u32=22):void{if(!ok){if(opened)close_case();finish(code);}}
function main():void{
 const mode=setup(4);opened=true;const initialized=initialize_case(mode,4,0);require(initialized==0,initialized);
 require(case_status()==0&&received_count()==0&&denied_count()==(mode==0?2:0));output('LEGACY_INITIALIZED '+mode.toString()+'\n');
 if(mode==0){require(poll_once()==9&&submit_once()==9&&close_case()==9);output('LEGACY_DENIED 2\n');require(control()==70);}
 else{
  let attempts:u32=0;while(true){require(control()==82);require(attempts++<4096,110);const code=poll_once();if(code==11){output('LEGACY_AGAIN\n');continue;}require(code==0,code);break;}
  const n=observed_length();require(case_status()==0&&received_count()==1&&denied_count()==0&&n>0&&n<=1536);
  let text='LEGACY_GUEST_STATE '+n.toString()+' ';const digits='0123456789abcdef';
  for(let i:u32=0;i<n;++i){const b=observed_byte(i);require(b<=255&&case_status()==0);text+=digits.charAt(<i32>(b>>4))+digits.charAt(<i32>(b&15));}output(text+'\n');
  require(control()==83);const sent=submit_once();require(sent==0,sent);require(submit_once()==9);output('LEGACY_SUBMITTED\n');
  require(control()==67);const closed=close_case();require(closed==0,closed);require(close_case()==9&&poll_once()==9&&submit_once()==9);output('LEGACY_CLOSED\n');require(control()==70);
 }
 opened=false;output('PASS_LEGACY_CASE\n');finish(0);
}
main();
