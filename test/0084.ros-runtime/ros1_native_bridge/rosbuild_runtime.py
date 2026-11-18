"""Resolve paths within an already verified rosbuild-era archive extraction."""
import os


def environment(extracted, distro, support):
    root=extracted/distro
    prefix=root/'opt/ros'/distro
    if not prefix.is_dir():
        raise ValueError('missing historical installation prefix')
    # Fuerte packages use installed Python trees. Earlier stack builds place
    # package sources and generated messages/services in separate directories.
    modules={'rospy','roslib','rosgraph','std_msgs','rosgraph_msgs','roscpp','rostime',
             'genpy','genmsg','rosmaster','rosbuild','catkin','rospkg','rosunit','rosparam','roslaunch'}
    paths=set()
    for init in prefix.rglob('__init__.py'):
        if init.parent.name in modules:
            paths.add(init.parent.parent)
    if not any((path/'rospy/__init__.py').is_file() for path in paths):
        raise ValueError('the release lacks its own Python client')
    if not any((path/'std_msgs/msg/__init__.py').is_file() for path in paths):
        raise ValueError('the release lacks its own generated standard messages')
    def order(path):
        return (0 if 'dist-packages' in path.parts else 1 if path.name=='src' else 2,str(path))
    python=support/'python2/usr'
    extras=[support/'melodic/usr/lib/python2.7/dist-packages',python/'lib/python2.7/dist-packages']
    roots=[p for p in (prefix/'ros',prefix/'share/ros',prefix) if p.is_dir()]
    search=[p for p in (prefix/'stacks',prefix/'share',*roots) if p.is_dir()]
    binaries=[p for p in (roots[0]/'bin',prefix/'bin') if p.is_dir()]
    libraries=[p for p in (prefix/'lib',roots[0]/'lib',roots[0]/'tools/rospack/lib') if p.is_dir()]
    return [python/'bin/python2.7','-B','-S'],dict(
        ROS_ROOT=str(roots[0]),ROS_DISTRO=distro,PYTHONHOME=str(python),WROS_ROSBUILD='1',
        ROS_PACKAGE_PATH=os.pathsep.join(map(str,search)),WROS_ROS1_PYTHON_PREFIX=str(prefix),
        PYTHONPATH=os.pathsep.join(map(str,[*sorted(paths,key=order),*extras])),
        PATH=os.pathsep.join([*map(str,binaries),'/usr/bin','/bin']),LD_LIBRARY_PATH=os.pathsep.join(map(str,libraries)))
