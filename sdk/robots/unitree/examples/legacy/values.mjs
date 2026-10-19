/** Complete generated ROS1 legacy values; no native layout, MD5 or hardware CRC.
 * Decode the real fixed-width zero value to construct every declared property,
 * then deliberately assign every LowCmd/LowState field and nested element.
 */
import {Ros1Unitree_legged_msgsLowCmdCodec as CommandCodec,Ros1Unitree_legged_msgsLowStateCodec as StateCodec} from './bounded_records.mjs';
export function expectedCommand(){
 const v=CommandCodec.decode(new Uint8Array(CommandCodec.maxWireBytes));
 v.head=[0xfe,0xef];v.levelFlag=0xff;v.frameReserve=7;v.SN=[0x10203040,0x50607080];v.version=[0x11223344,0x55667788];v.bandWidth=321;
 for(let i=0;i<20;++i){const m=v.motorCmd[i];m.mode=1+i%4;m.q=(i+1)/32;m.dq=-(i+1)/64;m.tau=(i+1)/128;m.Kp=(i+1)/8;m.Kd=(i+1)/256;for(let j=0;j<3;++j)m.reserve[j]=0x10000000+i*16+j+1;}
 v.bms.off=0;v.bms.reserve=[0x31,0x32,0x33];for(let i=0;i<40;++i)v.wirelessRemote[i]=(17+13*i)&255;
 v.reserve=0x24681357;v.crc=0x89abcdef;return v;
}
export function expectedState(){
 const v=StateCodec.decode(new Uint8Array(StateCodec.maxWireBytes));
 v.head=[0xfe,0xef];v.levelFlag=0xff;v.frameReserve=9;v.SN=[0x20304050,0x60708090];v.version=[0x22334455,0x66778899];v.bandWidth=654;
 for(let i=0;i<4;++i)v.imu.quaternion[i]=(i+1)/8;
 for(let i=0;i<3;++i){v.imu.gyroscope[i]=-(i+1)/16;v.imu.accelerometer[i]=(i+1)/32;v.imu.rpy[i]=-(i+1)/64;}v.imu.temperature=-9;
 for(let i=0;i<20;++i){const m=v.motorState[i];m.mode=1+i%4;m.q=(i+1)/32;m.dq=-(i+1)/64;m.ddq=(i+1)/128;m.tauEst=(i+1)/256;
  m.q_raw=-(i+1)/512;m.dq_raw=(i+1)/1024;m.ddq_raw=-(i+1)/2048;m.temperature=i-10;for(let j=0;j<2;++j)m.reserve[j]=0x20000000+i*16+j+1;}
 const b=v.bms;b.version_h=3;b.version_l=7;b.bms_status=11;b.SOC=75;b.current=-1234;b.cycle=456;b.BQ_NTC=[-2,3];b.MCU_NTC=[-4,5];for(let i=0;i<10;++i)b.cell_vol[i]=3100+7*i;
 for(let i=0;i<4;++i){v.footForce[i]=(i&1)?18+i:-17-i;v.footForceEst[i]=(i&1)?-34-i:33+i;
  const raw=v.eeForceRaw[i];raw.x=(i+1)/8;raw.y=-(i+1)/16;raw.z=(i+1)/32;const force=v.eeForce[i];force.x=-(i+1)/64;force.y=(i+1)/128;force.z=-(i+1)/256;}
 v.tick=42;for(let i=0;i<40;++i)v.wirelessRemote[i]=(29+11*i)&255;v.reserve=0x35792468;v.crc=0x13572468;
 v.position.x=-0.25;v.position.y=0.5;v.position.z=0.75;v.velocity.x=0.125;v.velocity.y=-0.0625;v.velocity.z=0.03125;
 v.velocity_w.x=-0.015625;v.velocity_w.y=0.0078125;v.velocity_w.z=-0.00390625;return v;
}
