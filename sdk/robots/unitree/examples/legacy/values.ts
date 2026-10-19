/** Complete actual generated ROS1 legacy values; no ROS2 wire layout/CRC packer.
 * Stage the entire fresh Noetic-bound family at the explicit import below.
 * Generated constructors initialize every fixed array/nested element.
 */
import {Ros1Unitree_legged_msgsLowCmd as Command,Ros1Unitree_legged_msgsLowState as State} from '../../../../custom-types/legacy-family/bounded_records';
export function expectedCommand():Command {
 const v=new Command();v.head[0]=0xfe;v.head[1]=0xef;v.levelFlag=0xff;v.frameReserve=7;
 v.SN[0]=0x10203040;v.SN[1]=0x50607080;v.version[0]=0x11223344;v.version[1]=0x55667788;v.bandWidth=321;
 for(let i=0;i<20;++i){const m=v.motorCmd[i];m.mode=<u8>(1+i%4);m.q=<f32>(i+1)/32;m.dq=-<f32>(i+1)/64;m.tau=<f32>(i+1)/128;m.Kp=<f32>(i+1)/8;m.Kd=<f32>(i+1)/256;
  for(let j=0;j<3;++j)m.reserve[j]=0x10000000+<u32>(i*16+j+1);}
 v.bms.off=0;for(let i=0;i<3;++i)v.bms.reserve[i]=<u8>(0x31+i);
 for(let i=0;i<40;++i)v.wirelessRemote[i]=<u8>(17+13*i);v.reserve=0x24681357;v.crc=0x89abcdef;return v;
}
export function expectedState():State {
 const v=new State();v.head[0]=0xfe;v.head[1]=0xef;v.levelFlag=0xff;v.frameReserve=9;
 v.SN[0]=0x20304050;v.SN[1]=0x60708090;v.version[0]=0x22334455;v.version[1]=0x66778899;v.bandWidth=654;
 for(let i=0;i<4;++i)v.imu.quaternion[i]=<f32>(i+1)/8;
 for(let i=0;i<3;++i){v.imu.gyroscope[i]=-<f32>(i+1)/16;v.imu.accelerometer[i]=<f32>(i+1)/32;v.imu.rpy[i]=-<f32>(i+1)/64;}v.imu.temperature=-9;
 for(let i=0;i<20;++i){const m=v.motorState[i];m.mode=<u8>(1+i%4);m.q=<f32>(i+1)/32;m.dq=-<f32>(i+1)/64;m.ddq=<f32>(i+1)/128;m.tauEst=<f32>(i+1)/256;
  m.q_raw=-<f32>(i+1)/512;m.dq_raw=<f32>(i+1)/1024;m.ddq_raw=-<f32>(i+1)/2048;m.temperature=<i8>(i-10);
  for(let j=0;j<2;++j)m.reserve[j]=0x20000000+<u32>(i*16+j+1);}
 const b=v.bms;b.version_h=3;b.version_l=7;b.bms_status=11;b.SOC=75;b.current=-1234;b.cycle=456;b.BQ_NTC[0]=-2;b.BQ_NTC[1]=3;b.MCU_NTC[0]=-4;b.MCU_NTC[1]=5;
 for(let i=0;i<10;++i)b.cell_vol[i]=<u16>(3100+7*i);
 for(let i=0;i<4;++i){v.footForce[i]=<i16>((i&1)?18+i:-17-i);v.footForceEst[i]=<i16>((i&1)?-34-i:33+i);
  const raw=v.eeForceRaw[i];raw.x=<f32>(i+1)/8;raw.y=-<f32>(i+1)/16;raw.z=<f32>(i+1)/32;
  const force=v.eeForce[i];force.x=-<f32>(i+1)/64;force.y=<f32>(i+1)/128;force.z=-<f32>(i+1)/256;}
 v.tick=42;for(let i=0;i<40;++i)v.wirelessRemote[i]=<u8>(29+11*i);v.reserve=0x35792468;v.crc=0x13572468;
 v.position.x=-0.25;v.position.y=0.5;v.position.z=0.75;v.velocity.x=0.125;v.velocity.y=-0.0625;v.velocity.z=0.03125;
 v.velocity_w.x=-0.015625;v.velocity_w.y=0.0078125;v.velocity_w.z=-0.00390625;return v;
}
