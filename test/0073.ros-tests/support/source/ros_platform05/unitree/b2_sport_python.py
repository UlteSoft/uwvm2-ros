"""Full selected B2 value vectors; host execution alone is not Guest evidence."""
import sys
from pathlib import Path

# Explicit delivered modules, never a fake backend or substituted ROS package.
sdk=Path(sys.argv[1]).resolve(strict=True)
overlay=Path(sys.argv[2]).resolve(strict=True)
sys.path[:0]=[str(overlay/'python'),str(sdk/'include/platform/unitree/py/ros2/unitree-api-complete'),
    str(sdk/'include/platform/ros/py')]
from bounded_records import Ros2Unitree_apiMsgRequest as Request
from b2_sport import OPERATIONS, PathPoint, B2SportRequestBuilder, prepare


def main():
    request=Request()
    request.header.identity.id=9007199254740993
    request.header.identity.api_id=-77
    request.header.lease.id=-9007199254740993
    request.header.policy.priority=-101
    request.header.policy.noreply=True
    request.parameter='keep'
    request.binary=[0,255,1,128]
    before=request.encode()
    path=[PathPoint(float(i),0.0,-0.0,0.0,0.0,0.0,0.0) for i in range(30)]
    builder=B2SportRequestBuilder()
    assert len(OPERATIONS)==21
    for name,(api,kind) in OPERATIONS.items():
        arguments=() if kind=='none' else (1.25,-0.0,0.5) if kind in ('xyz','position') else \
            (-2147483648,) if kind=='integer' else (True,) if kind=='boolean' else \
            (-0.125,) if kind=='height' else (path,)
        result=getattr(builder,name)(request,*arguments)
        assert result.header.identity.api_id==api and request.encode()==before
        assert result.header is not request.header and result.binary is not request.binary
        print(name+'\t'+result.encode().hex())
        result.binary[0]=99
        assert request.encode()==before
    invalid_paths=(path[:-1],path+[path[0]],path[:-1]+[PathPoint(29.0,0,0,0,0,0,float('nan'))],
        [PathPoint(*([3.4028234663852886e38]*7)) for _ in range(30)])
    for invalid in invalid_paths:
        try:prepare(request,'TrajectoryFollow',invalid)
        except (ValueError,OverflowError):pass
        else:raise AssertionError('invalid/oversized complete trajectory accepted')
        assert request.encode()==before
    try:prepare(request,'AutoRecoveryGet')
    except ValueError:pass
    else:raise AssertionError('Go2-only method admitted to B2')
    assert request.encode()==before
    print('B2_SPORT_LOCAL_OK operations=21 negative_cases=5')


if __name__=='__main__':main()
