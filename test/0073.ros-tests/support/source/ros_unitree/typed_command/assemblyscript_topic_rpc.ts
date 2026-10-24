/** Same-main AS API Request/Response test. selected_case re-exports the actual
 * complete typed API example. No service or firmware endpoint is introduced.
 * Actual Runtime main rights and independent native UTN1 observations are
 * required. --exportStart typed_command_entry moves this explicit initializer
 * into an ordinary void export; the actual binary must have no start section.
 */
import {initialize_case,submit_once,poll_once,case_status,received_count,denied_verified,close_case,response_code} from './selected_case';
export {initialize_case,submit_once,poll_once,case_status,received_count,denied_verified,close_case,response_code} from './selected_case';
import {setup,control,output,finish} from './protocol';
let opened=false;
function require(ok:bool,code:u32=22):void {if(!ok){if(opened)close_case();finish(code);}}
function main():void {
  const mode=setup(3);const code=initialize_case(mode,1,1);opened=true;require(code==0,code);
  require(case_status()==0&&received_count()==0&&denied_verified()==(mode==0?1:0));output('TYPED_INITIALIZED '+mode.toString()+'\n');
  if(mode==0){
    require(poll_once()==9&&close_case()==9&&submit_once()==9);output('TYPED_DENIED_VERIFIED\n');require(control()==70);
  }else{
    require(control()==83);const sent=submit_once();require(sent==0,sent);require(submit_once()==9);output('TYPED_SUBMITTED\n');
    let polls:u32=0;
    while(true){
      require(control()==82);require(polls++<4096,110);const result=poll_once();
      if(result==11){output('TYPED_AGAIN\n');continue;}
      require(result==0,result);require(case_status()==0&&received_count()==1&&denied_verified()==0);break;
    }
    const response=response_code();require(case_status()==0);output('TYPED_RPC '+response.toString()+' 1 0\n');
    require(control()==67);const closed=close_case();require(closed==0,closed);require(close_case()==9&&poll_once()==9&&submit_once()==9);
    output('TYPED_CLOSED\n');require(control()==70);
  }
  opened=false;output('PASS_TYPED_CASE\n');finish(0);
}
main();
