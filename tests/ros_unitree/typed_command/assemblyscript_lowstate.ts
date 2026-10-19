/** Real AS same-main command entry. The owned selected_case module is a literal
 * re-export of the MF-selected GO or HG example, not a mock or wire substitute.
 * The build supplies one exact literal SELECTED_FAMILY=1|2 through a generated
 * selected_family module derived from its closed product row. All imported
 * case functions are the unchanged actual typed SDK implementation.
 *
 * Compile this file with --exportStart typed_command_entry. Runtime explicitly
 * selects that actual void local export as main entry; no implicit start or
 * preloaded application is allowed. Its initializer first constructs AS globals
 * and then executes this finite protocol. No second initializer call occurs.
 */
import {initialize_case,poll_once,case_status,received_count,denied_verified,close_case,observed_tick,observed_crc,motor_word} from './selected_case';
export {initialize_case,poll_once,case_status,received_count,denied_verified,close_case,observed_tick,observed_crc,motor_word} from './selected_case';
import {SELECTED_FAMILY} from './selected_family';
import {setup,control,output,finish} from './protocol';
let opened=false;
function require(ok:bool,code:u32=22):void {if(!ok){if(opened)close_case();finish(code);}}
function main():void {
  require(SELECTED_FAMILY==1||SELECTED_FAMILY==2);const mode=setup(SELECTED_FAMILY);
  const code=initialize_case(mode,SELECTED_FAMILY,0);opened=true;require(code==0,code);
  require(case_status()==0&&received_count()==0&&denied_verified()==(mode==0?1:0));
  output('TYPED_INITIALIZED '+mode.toString()+'\n');
  if(mode==0){
    require(poll_once()==9&&close_case()==9);output('TYPED_DENIED_VERIFIED\n');require(control()==70);
  }else{
    let polls:u32=0;
    while(true){
      require(control()==82);require(polls++<4096,110);const result=poll_once();
      if(result==11){output('TYPED_AGAIN\n');continue;}
      require(result==0,result);require(case_status()==0&&received_count()==1&&denied_verified()==0);break;
    }
    const slots:u32=SELECTED_FAMILY==1?20:35;
    let value='TYPED_STATE '+observed_tick().toString()+' '+observed_crc().toString()+' '+slots.toString();
    for(let slot:u32=0;slot<slots;++slot)for(let field:u32=0;field<3;++field)value+=' '+motor_word(slot,field).toString();
    require(case_status()==0);output(value+'\n');
    require(control()==67);const result=close_case();require(result==0,result);require(close_case()==9&&poll_once()==9);
    output('TYPED_CLOSED\n');require(control()==70);
  }
  opened=false;output('PASS_TYPED_CASE\n');finish(0);
}
main();
