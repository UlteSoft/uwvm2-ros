/** Stage as header.ts beside selected ROS1 binding.ts. */
import {RobotMsg} from './binding';
export function headerMatches(v:RobotMsg):bool{return v.header.seq==19&&v.header.stamp.sec==42&&v.header.stamp.nsec==300000000;}
