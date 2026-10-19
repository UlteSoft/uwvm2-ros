/** Real embedded-Wasm JS command. All imports are admitted original modules.
 * Root stages ONE exact mode source as mode.mjs and selects native backend1/2
 * in the maintained QuickJS Guest producer. No Node, OS module or host mock.
 */
import {createGuestSession,readGuestByte,writeGuestOutput} from '../../../../javascript/guest.mjs';
import {runCIOState} from './lesson.mjs';
import {MODE} from './mode.mjs';
const result=await runCIOState(createGuestSession(),MODE,()=>{
  const byte=readGuestByte();if(!Number.isInteger(byte)||byte<0||byte>255)throw new TypeError('actual basic input byte');
  return String.fromCharCode(byte);
},text=>writeGuestOutput(new TextEncoder().encode(text)));
if(result.status!==0)throw new Error('CIOState lesson refused with status '+result.status);
export {result};
