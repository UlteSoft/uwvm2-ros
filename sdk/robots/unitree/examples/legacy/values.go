//go:build tinygo && (wasm || tinygo.wasm) && ros1 && !ros2

// Complete actual generated legacy Guest values, not native ABI/hardware CRC.
package main
import vendor "example.local/uwvm-unitree-legacy/customtypes"
func expectedCommand() vendor.Ros1Unitree_legged_msgsLowCmd {
 var v vendor.Ros1Unitree_legged_msgsLowCmd
 v.Head=[2]uint8{0xfe,0xef};v.LevelFlag=0xff;v.FrameReserve=7
 v.SN=[2]uint32{0x10203040,0x50607080};v.Version=[2]uint32{0x11223344,0x55667788};v.BandWidth=321
 for i:=0;i<20;i++ {m:=&v.MotorCmd[i];m.Mode=uint8(1+i%4);m.Q=float32(i+1)/32;m.Dq=-float32(i+1)/64;m.Tau=float32(i+1)/128;m.Kp=float32(i+1)/8;m.Kd=float32(i+1)/256
  for j:=0;j<3;j++ {m.Reserve[j]=0x10000000+uint32(i*16+j+1)}}
 v.Bms.Off=0;v.Bms.Reserve=[3]uint8{0x31,0x32,0x33};for i:=0;i<40;i++ {v.WirelessRemote[i]=uint8(17+13*i)}
 v.Reserve=0x24681357;v.Crc=0x89abcdef;return v
}
func expectedState() vendor.Ros1Unitree_legged_msgsLowState {
 var v vendor.Ros1Unitree_legged_msgsLowState
 v.Head=[2]uint8{0xfe,0xef};v.LevelFlag=0xff;v.FrameReserve=9
 v.SN=[2]uint32{0x20304050,0x60708090};v.Version=[2]uint32{0x22334455,0x66778899};v.BandWidth=654
 for i:=0;i<4;i++ {v.Imu.Quaternion[i]=float32(i+1)/8}
 for i:=0;i<3;i++ {v.Imu.Gyroscope[i]=-float32(i+1)/16;v.Imu.Accelerometer[i]=float32(i+1)/32;v.Imu.Rpy[i]=-float32(i+1)/64};v.Imu.Temperature=-9
 for i:=0;i<20;i++ {m:=&v.MotorState[i];m.Mode=uint8(1+i%4);m.Q=float32(i+1)/32;m.Dq=-float32(i+1)/64;m.Ddq=float32(i+1)/128;m.TauEst=float32(i+1)/256
  m.Q_raw=-float32(i+1)/512;m.Dq_raw=float32(i+1)/1024;m.Ddq_raw=-float32(i+1)/2048;m.Temperature=int8(i-10)
  for j:=0;j<2;j++ {m.Reserve[j]=0x20000000+uint32(i*16+j+1)}}
 b:=&v.Bms;b.Version_h=3;b.Version_l=7;b.Bms_status=11;b.SOC=75;b.Current=-1234;b.Cycle=456;b.BQ_NTC=[2]int8{-2,3};b.MCU_NTC=[2]int8{-4,5}
 for i:=0;i<10;i++ {b.Cell_vol[i]=uint16(3100+7*i)}
 for i:=0;i<4;i++ {if i&1!=0 {v.FootForce[i]=int16(18+i);v.FootForceEst[i]=int16(-34-i)}else{v.FootForce[i]=int16(-17-i);v.FootForceEst[i]=int16(33+i)}
  raw:=&v.EeForceRaw[i];raw.X=float32(i+1)/8;raw.Y=-float32(i+1)/16;raw.Z=float32(i+1)/32
  force:=&v.EeForce[i];force.X=-float32(i+1)/64;force.Y=float32(i+1)/128;force.Z=-float32(i+1)/256}
 v.Tick=42;for i:=0;i<40;i++ {v.WirelessRemote[i]=uint8(29+11*i)};v.Reserve=0x35792468;v.Crc=0x13572468
 v.Position.X=-0.25;v.Position.Y=0.5;v.Position.Z=0.75;v.Velocity.X=0.125;v.Velocity.Y=-0.0625;v.Velocity.Z=0.03125
 v.Velocity_w.X=-0.015625;v.Velocity_w.Y=0.0078125;v.Velocity_w.Z=-0.00390625;return v
}
