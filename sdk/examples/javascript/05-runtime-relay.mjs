/**
 * @file 05-runtime-relay.mjs
 * @brief Checked host-driver lesson over a separately supervised real Worker.
 *
 * Usage: node 05-runtime-relay.mjs 1|2 positive|absent
 * fd3 reads exclusive Worker binary stdout; fd4 writes Worker binary stdin.
 * Driver stdin receives P then at most 200 single-byte R gates. Driver stdout
 * carries only fixed SDK markers. Worker audit stderr is a separate owned log.
 * The supervisor starts/authenticates/maps actors; this lesson spawns nothing,
 * inherits no Runtime authority and never connects native ROS or Gateway IPC.
 * Compilation, real delivery and default-policy qualification remain external.
 */
import { createReadStream, createWriteStream } from 'node:fs';
import { AsyncSession, AsyncNode, StringMessage, Status } from '../../javascript/async.mjs';

const OUT='roswasm-js-relay-out',IN='roswasm-js-relay-in',WARMUP='warmup-js-relay';
const SENTINEL=0x8877665544332211n,MAX_POLLS=200;
function require(condition,message) {if(!condition)throw new Error(message);}
function same(a,b) {return a.length===b.length&&a.every((byte,index)=>byte===b[index]);}
function marker(text) {process.stdout.write(text+'\n');}

/** Each control byte is supplied only after the preceding fixed marker. This
 * reader preserves short chunks, rejects excess buffering/EOF and creates no
 * Guest clock or background native operation. Driver control is independent
 * of the binary Worker pipes; arbitrary text is not interpreted as commands.
 */
class Gates {
  constructor(stream) {this.reader=stream[Symbol.asyncIterator]();this.bytes=new Uint8Array();}
  async read(expected) {
    if(!this.bytes.length) {
      const next=await this.reader.next();require(!next.done,'Control EOF before required gate');
      require(next.value instanceof Uint8Array&&next.value.length>0&&next.value.length<=16,'Bounded control input required');
      this.bytes=next.value.slice();
    }
    const actual=this.bytes[0];this.bytes=this.bytes.slice(1);
    require(actual===expected.charCodeAt(0),'Unexpected control gate');
  }
}

/** Raw denial test protects both the output slot and 64 output bytes. These
 * exact values are observations of the actual import result, not wrapper-only
 * errors. A permitted String alias does not authorize a second alias.
 */
async function denied(session,alias) {
  const input=StringMessage.encode(alias).subarray(4),s=session.layout;
  const reply=await session.raw('capability_open',[0,s.input,BigInt(input.length),s.slot],
    {input,slotSeed:SENTINEL,snapshot:64});
  require(reply.status===Status.DENIED&&reply.slot===SENTINEL&&reply.snapshot.length===64&&reply.snapshot.every(byte=>byte===0xa5),
    'Denial13 must preserve exact protected slot/output');
}

/** Positive exchange uses only pub/sub String grants, plus the independent
 * ungranted alias test. The exact canonical record sizes are warmup19, out24,
 * in23 bytes. Native MATCHED must precede P; each R performs exactly one poll.
 */
async function positive(session,gates) {
  const node=new AsyncNode(session,'sdk-relay-example');let received=0;
  try {
    await denied(session,'ungranted');
    const pub=await node.createPublisher('pub',StringMessage);
    const expected=StringMessage.encode(IN);
    const sub=await node.createSubscription('sub',StringMessage,(value,result)=>{
      require(value===IN&&result.required===23n&&same(result.bytes,expected),'Exact complete String reply required');
      require(++received===1,'Duplicate reply refused');
    });
    require(StringMessage.encode(WARMUP).length===19&&StringMessage.encode(OUT).length===24&&expected.length===23,'Canonical literal lengths');
    require(await pub.publish(WARMUP)===0,'Warmup publication refused');
    marker('SDK_ENDPOINTS_CREATED');await gates.read('P');
    require(await pub.publish(OUT)===0,'Final publication refused');marker('SDK_FINAL_SENT');
    for(let polls=0;polls<MAX_POLLS&&received===0;polls++) {
      await gates.read('R');const status=await sub.spinOnce(23);
      require(status===0||status===Status.AGAIN,'Poll accepts only success or11');
      if(status===Status.AGAIN)marker('SDK_POLL_AGAIN');
    }
    require(received===1,'Bounded poll budget exhausted without delivery');marker('SDK_DELIVERED');
    const old=pub.capability.handle;
    require(await pub.close()===0&&await sub.close()===0,'Explicit resource close refused');
    require(await session.write('topic_publish',old,StringMessage.encode(OUT))===Status.BAD_HANDLE,'Actual stale handle must return9');
    require((await node.close()).length===0,'No additional retained handle expected');
    await session.shutdown();marker('PASS_SDK_DELIVERY');
  } catch(error) {
    // Cleanup is attempted once. Its failures are retained beside the original
    // error and cannot turn this incomplete exchange into a successful marker.
    if(!node.closed) {
      try {error.cleanup=await node.close();}catch(cleanup){error.cleanup=cleanup;}
    }
    throw error;
  }
}

/** Empty ROS policy row: every plausible alias is denied without publication.
 * Native absence and actual Worker exit are verified by the supervisor.
 */
async function absent(session) {
  for(const alias of ['pub','sub','ungranted'])await denied(session,alias);
  await session.shutdown();marker('PASS_SDK_DEFAULT_DENIED');
}

const [versionText,mode,...extra]=process.argv.slice(2),version=Number(versionText);
require(extra.length===0&&(versionText==='1'||versionText==='2')&&(mode==='positive'||mode==='absent'),'Explicit backend and positive|absent mode required');
const readable=createReadStream(null,{fd:3,autoClose:false,highWaterMark:8192});
const writable=createWriteStream(null,{fd:4,autoClose:false,highWaterMark:8192});
try {
  const session=await AsyncSession.attach({version,readable,writable,timeoutMs:10000});
  if(mode==='positive')await positive(session,new Gates(process.stdin));else await absent(session);
} catch {
  // Fixed public failure only: no alias/body/credentials/dynamic exception.
  process.stderr.write('FAIL_SDK_RELAY\n');process.exitCode=1;
} finally {
  readable.destroy();writable.destroy();process.stdin.pause();
}
