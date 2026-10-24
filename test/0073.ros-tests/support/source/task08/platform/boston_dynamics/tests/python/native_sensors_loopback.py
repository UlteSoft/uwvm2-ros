"""Actual SDK/gRPC sensor loopback; no robot, ROS, Guest or mocked client.

The finite test peer implements original generated services on 127.0.0.1 only.
Its complete protobuf responses and actual transport limits are compared with
the genuine SDK client/owner result. This is not hardware sensor evidence.
"""
import argparse
from concurrent.futures import ThreadPoolExecutor
import hashlib
import json
from pathlib import Path
import sys
import time


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--dependencies', type=Path, required=True)
    parser.add_argument('--source', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    sys.path[:0] = [str(args.dependencies), str(args.source/'sdk/origin/robots')]
    import grpc
    from bosdyn.api import header_pb2, image_pb2, image_service_pb2_grpc
    from bosdyn.api import point_cloud_pb2, point_cloud_service_pb2_grpc
    from bosdyn.client.image import ImageClient
    from bosdyn.client.point_cloud import PointCloudClient
    from mobile_manipulation.spot.native_sensors import SensorPolicy, SpotSensorOwner

    responses, requests, rows = {}, [], []

    def response(message, request, name):
        message.header.request_header.CopyFrom(request.header)
        message.header.error.code = header_pb2.CommonError.CODE_OK
        responses[name] = message.SerializeToString(deterministic=True)
        requests.append((name, request.SerializeToString(deterministic=True)))
        return message

    class Images(image_service_pb2_grpc.ImageServiceServicer):
        def ListImageSources(self, request, context):
            result=image_pb2.ListImageSourcesResponse()
            value=result.image_sources.add()
            value.name='front';value.rows=2;value.cols=2
            value.image_type=image_pb2.ImageSource.IMAGE_TYPE_VISUAL
            value.pinhole.intrinsics.focal_length.x=123.5
            return response(result,request,'image.sources')

        def GetImage(self, request, context):
            result=image_pb2.GetImageResponse()
            value=result.image_responses.add()
            value.status=image_pb2.ImageResponse.STATUS_OK
            value.source.name='front';value.source.rows=2;value.source.cols=2
            value.shot.frame_name_image_sensor='front'
            value.shot.acquisition_time.seconds=1 << 33
            value.shot.acquisition_time.nanos=123456789
            edge=value.shot.transforms_snapshot.child_to_parent_edge_map['front']
            edge.parent_frame_name='vision';edge.parent_tform_child.rotation.w=1
            edge.parent_tform_child.position.x=0.125
            value.shot.image.rows=2;value.shot.image.cols=2
            value.shot.image.pixel_format=image_pb2.Image.PIXEL_FORMAT_GREYSCALE_U8
            value.shot.image.format=image_pb2.Image.FORMAT_RAW
            value.shot.image.data=b'\0\xff12'
            # A valid unknown original protobuf field must also survive the
            # actual generated gRPC serializer and complete SDK response.
            result.MergeFromString(b'\xf8\x7f\x07')
            return response(result,request,'image.get')

    class Clouds(point_cloud_service_pb2_grpc.PointCloudServiceServicer):
        def ListPointCloudSources(self, request, context):
            result=point_cloud_pb2.ListPointCloudSourcesResponse()
            result.point_cloud_sources.add().name='lidar'
            return response(result,request,'cloud.sources')

        def GetPointCloud(self, request, context):
            result=point_cloud_pb2.GetPointCloudResponse()
            value=result.point_cloud_responses.add()
            value.status=point_cloud_pb2.PointCloudResponse.STATUS_OK
            value.point_cloud.source.name='lidar'
            value.point_cloud.source.frame_name_sensor='lidar'
            value.point_cloud.source.acquisition_time.seconds=1 << 33
            value.point_cloud.num_points=1
            value.point_cloud.encoding=point_cloud_pb2.PointCloud.ENCODING_XYZ_32F
            value.point_cloud.data=b'\0\0\x80?\0\0\0@\0\0@@'
            return response(result,request,'cloud.get')

    pool=ThreadPoolExecutor(max_workers=2)
    server=grpc.server(pool,options=(('grpc.max_receive_message_length',65536),
                                    ('grpc.max_send_message_length',65536)))
    image_service_pb2_grpc.add_ImageServiceServicer_to_server(Images(),server)
    point_cloud_service_pb2_grpc.add_PointCloudServiceServicer_to_server(Clouds(),server)
    port=server.add_insecure_port('127.0.0.1:0')
    assert port > 0
    server.start()
    channel=grpc.insecure_channel('127.0.0.1:'+str(port),options=(
        ('grpc.max_receive_message_length',65536),('grpc.max_send_message_length',65536),
        ('grpc.enable_retries',0)))
    try:
        image_client,cloud_client=ImageClient(),PointCloudClient()
        image_client.channel=channel;cloud_client.channel=channel
        owner,robot=object(),object()
        policy=SensorPolicy(authenticated_owner=owner,authenticated_robot=robot,
            operations=frozenset(('image.sources','image.get','cloud.sources','cloud.get')),
            image_sources=('front',),cloud_sources=('lidar',),
            expires_at_ns=time.monotonic_ns()+30_000_000_000,
            maximum_rpc_ns=2_000_000_000,maximum_request_bytes=4096,
            maximum_response_bytes=65536,maximum_sources_per_call=2)
        native=SpotSensorOwner(image_client,cloud_client,policy=policy,transport_receive_limit=65536)

        def refused(name,call,kind=PermissionError):
            before=len(requests)
            try:call()
            except kind:pass
            else:raise AssertionError(name+' unexpectedly returned')
            assert len(requests)==before
            rows.append(dict(name=name,result='PASS',native_rpcs=0))

        def invoke(op,correlation,request,who=owner,which=robot):
            return native.invoke_from_authenticated_host(who,which,op,correlation,
                time.monotonic_ns()+1_000_000_000,request)

        empty=SpotSensorOwner(image_client,cloud_client)
        refused('default-empty native policy',lambda:empty.invoke_from_authenticated_host(
            owner,robot,'image.sources',1,time.monotonic_ns()+1_000_000_000,image_pb2.ListImageSourcesRequest()))
        refused('foreign authenticated owner',lambda:invoke('image.sources',1,
            image_pb2.ListImageSourcesRequest(),who=object()))
        refused('foreign authenticated robot',lambda:invoke('image.sources',1,
            image_pb2.ListImageSourcesRequest(),which=object()))
        image_request=image_pb2.GetImageRequest()
        image_request.image_requests.add(image_source_name='front',quality_percent=87,resize_ratio=0)
        cloud_request=point_cloud_pb2.GetPointCloudRequest()
        cloud_request.point_cloud_requests.add(point_cloud_source_name='lidar',downsample_rate=2)
        for index,(operation,request) in enumerate((
                ('image.sources',image_pb2.ListImageSourcesRequest()),
                ('image.get',image_request),
                ('cloud.sources',point_cloud_pb2.ListPointCloudSourcesRequest()),
                ('cloud.get',cloud_request)),start=1):
            value=invoke(operation,index,request)
            assert value.SerializeToString(deterministic=True)==responses[operation]
            assert requests[-1][1]==request.SerializeToString(deterministic=True)
            rows.append(dict(name=operation+' full original request/response',result='PASS',native_rpcs=1))
        refused('correlation is never replayed',lambda:invoke('image.get',2,image_request),ValueError)
        other=image_pb2.GetImageRequest()
        other.image_requests.add(image_source_name='rear')
        refused('ungranted original source',lambda:invoke('image.get',5,other))
        native.retire_native_only(owner)
        refused('retired native sensor dispatch',lambda:invoke('image.get',6,image_request))
        frames=args.output.parent/'sensor-frames'
        frames.mkdir()
        captures=[]
        for operation,request_bytes in requests:
            for direction,body in (('request',request_bytes),('response',responses[operation])):
                path=frames/(operation+'.'+direction+'.pb')
                with path.open('xb') as output:output.write(body)
                captures.append(dict(path=str(path),operation=operation,direction=direction,
                    bytes=len(body),sha256=hashlib.sha256(body).hexdigest()))
        report=dict(schema='uwvm-task08-spot-sensors-loopback-v2',cases=rows,
            passed=len(rows),denominator=len(rows),native_sdk_rpcs=len(requests),
            transport='actual generated gRPC services on 127.0.0.1 only',
            full_original_request_response_bytes_compared=True,frames=captures,default_authority=[],
            robot_cases=0,ros_cases=0,gateway_cases=0,wasm_guest_cases=0)
        args.output.write_text(json.dumps(report,indent=2,sort_keys=True)+'\n')
        print(json.dumps(dict(passed=len(rows),denominator=len(rows),native_sdk_rpcs=len(requests))))
    finally:
        channel.close()
        assert server.stop(0).wait(timeout=2),'actual gRPC server did not stop'
        pool.shutdown(wait=True,cancel_futures=True)


if __name__=='__main__':main()
