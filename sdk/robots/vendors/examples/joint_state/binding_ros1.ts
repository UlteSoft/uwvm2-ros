/** Stage this exact source as binding.ts for a ROS1 build only. */
export {Session,Capability,NativeApi} from '../../../../assemblyscript/ros1/roswasm';
export {Ros1Sensor_msgsJointState as JointState,Ros1Sensor_msgsJointStateCodec as JointCodec} from '../../../../custom-types/generated-official-standard/bounded_records';
export const ROS_VERSION:u32=1;
