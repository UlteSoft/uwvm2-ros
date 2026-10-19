/** Real QuickJS-in-Wasm ROS1 command, not Node or a host ROS shim.
 * All SDK calls run in this actual main module. The real closed engine module
 * map must include the entire native-bound eleven-type provider and SDK.
 * No module evaluation opens an alias. main() is the finite explicit driver.
 */
import {createGuestSession,Node,readGuestByte,writeGuestOutput} from '../../../../javascript/guest.mjs';
import {Ros1Unitree_legged_msgsLowStateCodec as StateCodec,Ros1Unitree_legged_msgsLowCmdCodec as CommandCodec} from './bounded_records.mjs';
import {expectedState,expectedCommand} from './values.mjs';
let started=false,closed=false,submitted=false,status=0,polls=0,received=0,denials=0;
let node=null,publisher=null,subscription=null,observed=new Uint8Array(0);
function code(error){return Number.isInteger(error?.code)&&error.code>0&&error.code<=0xffffffff?error.code:typeof error==='number'&&error>0?error:22;}
function closeAll(first=0){
 if(closed)return first||9;closed=true;
 try{if(node)for(const result of node.close())if(!first&&result)first=result;}catch(error){if(!first)first=code(error);}
 publisher=null;subscription=null;status=first;return first;
}
function check(ok,value=22){if(!ok){const error=new Error('legacy command refused');error.code=value;throw error;}}
function initialize(mode){
 check(!started&&!closed);started=true;check(mode===0||mode===1);
 const session=createGuestSession();check(session.version===1,95);node=new Node(session,'legacy-typed-example');
 if(mode===0){
  for(const alias of ['legacy-lowcmd','legacy-lowstate']){
   let owner=null;try{owner=node.open(alias);}catch(error){if(code(error)===13){++denials;continue;}throw error;}
   if(owner)owner.close();check(false);
  }check(denials===2);check(closeAll()===0);return;
 }
 publisher=node.createPublisher('legacy-lowcmd',CommandCodec);
 // Real constructor calls topic_subscribe now, BEFORE initialized/native M.
 subscription=node.createSubscription('legacy-lowstate',StateCodec,value=>{
  check(received===0);const actual=StateCodec.encode(value),expected=StateCodec.encode(expectedState());
  check(actual.length>0&&actual.length<=1536&&actual.length===expected.length);
  for(let i=0;i<actual.length;++i)check(actual[i]===expected[i]);observed=actual;received=1;
 });
}
function pollOnce(){check(!closed&&received===0,9);check(polls++<4096,110);const result=subscription.spinOnce();if(result===11)return 11;check(result===0,result);check(received===1);return 0;}
function submit(){check(!closed&&received===1&&!submitted,9);const result=publisher.publish(expectedCommand());check(result===0,result);submitted=true;}
function expectClosed(operation){let refused=false;try{operation();}catch(error){check(code(error)===9);refused=true;}check(refused);}
function input(){const value=readGuestByte();check(Number.isInteger(value)&&value>=0&&value<=255);return value;}
function output(text){check(typeof text==='string'&&text.length<=4096);const bytes=new Uint8Array(text.length);for(let i=0;i<text.length;++i){const c=text.charCodeAt(i);check(c<128);bytes[i]=c;}const result=writeGuestOutput(bytes);check(result===0,result);}
function le32(b,o){return (b[o]|b[o+1]<<8|b[o+2]<<16|b[o+3]<<24)>>>0;}
function control(letter){check(input()===letter.charCodeAt(0));}
export function main(){
 try{
  output('READY_LEGACY_MAIN\n');const b=new Uint8Array(16);for(let i=0;i<16;++i)b[i]=input();
  check(b[0]===85&&b[1]===76&&b[2]===71&&b[3]===49&&le32(b,4)===4&&le32(b,8)<=1&&le32(b,12)===0);
  const mode=le32(b,8);initialize(mode);output('LEGACY_INITIALIZED '+mode+'\n');
  if(mode===0){check(closed&&denials===2&&received===0&&status===0);expectClosed(pollOnce);expectClosed(submit);check(closeAll()===9);output('LEGACY_DENIED 2\n');control('F');}
  else{
   for(let i=0;;++i){check(i<4096,110);control('R');if(pollOnce()===11){output('LEGACY_AGAIN\n');continue;}break;}
   let hex='';const digits='0123456789abcdef';for(let i=0;i<observed.length;++i)hex+=digits[observed[i]>>>4]+digits[observed[i]&15];
   output('LEGACY_GUEST_STATE '+observed.length+' '+hex+'\n');
   control('S');submit();expectClosed(submit);output('LEGACY_SUBMITTED\n');control('C');check(closeAll()===0);expectClosed(pollOnce);expectClosed(submit);check(closeAll()===9&&status===0);output('LEGACY_CLOSED\n');control('F');
  }
  output('PASS_LEGACY_CASE\n');return 0;
 }catch(error){const first=code(error);if(!closed)closeAll(first);throw error;}
 finally{if(!closed){const result=closeAll();check(result===0,result);}}
}
