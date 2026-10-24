"""UNEXECUTED complete-value/association models, run in the real frozen Guest.

Locally constructed DATA is not a Native receipt or transport qualification.
These independent struct fixtures never authorize an endpoint or call imports.
"""
import struct
from roswasm.owned_service.wire import (receipt, delivery, WireError, CLIENT,
    SERVICE, U64_MAX, same_receipt)

SOURCE=(1<<32)|1
PEER=(2<<32)|2


def qos():
    data=bytearray(80)
    struct.pack_into('<IIIIQ',data,0,1,2,1,2,10)
    struct.pack_into('<QQQQQQ',data,24,1,999999999,2,0,3,1)
    data[72]=1
    return bytes(data)


def endpoint(kind=CLIENT,operation=1):
    data=bytearray(256)
    struct.pack_into('<4sHBBI',data,0,b'UMRE',1,operation,2,256)
    data[12],data[13]=kind,2
    data[14]=32 if kind==CLIENT else 0
    struct.pack_into('<QQQQQ',data,16,SOURCE,9,10,11,12)
    data[64:144]=data[144:224]=qos()
    if kind==CLIENT:data[224:256]=bytes(range(32))
    return bytes(data)


def packet(*,kind=CLIENT,sequence=1,nonce=1,ticket=U64_MAX,body=b'\0\0\0\0',control=0):
    data=bytearray(352+len(body))
    struct.pack_into('<4sHBBI',data,0,b'UMRC',1,5,2,160)
    data[12],data[13]=kind,16
    struct.pack_into('<QQQQQQQQQQQQQ',data,16,SOURCE,9,10,11,12,PEER,ticket,sequence,nonce,0,len(data),192+len(body),0)
    struct.pack_into('<4sHBBII',data,160,b'URMI',1,control or (3 if kind==CLIENT else 2),2,192,0 if control else 35)
    struct.pack_into('<QQQQQQQqqQQqQ',data,176,SOURCE,9,10,11,12,sequence,ticket,0 if control else -(1<<63),0 if control else (1<<63)-1,U64_MAX,U64_MAX,0 if control else -1,len(body))
    if control:
        data[282]=1 if control==7 else 0
        struct.pack_into('<I',data,284,90 if control==6 else 0)
    else:
        data[281]=16
        # A zero-valued full writer GUID is legal actual data, not missing.
        data[320:336]=bytes(16)
    data[352:]=body
    return bytes(data)


def refused(call):
    try:call()
    except WireError:return
    raise AssertionError('malformed model accepted')


def main():
    expected=(qos(),qos())
    ep=receipt(endpoint(),SOURCE,CLIENT,1,expected)
    assert len(ep.gid)==32
    inspected=receipt(endpoint(operation=2),SOURCE,CLIENT,2,expected)
    assert same_receipt(ep,inspected)
    for offset in (4,7,8,12,13,14,15,16,24,32,40,48,56,64,73,136,217):
        bad=bytearray(endpoint());bad[offset]^=255
        refused(lambda:receipt(bytes(bad),SOURCE,CLIENT,1,expected))
    accepted=delivery(packet(),ep,PEER,4,0,0)
    assert accepted.borrow.ticket==U64_MAX and accepted.information.native_sequence==-1
    assert accepted.information.source_timestamp==-(1<<63) and accepted.information.received_timestamp==(1<<63)-1
    assert accepted.information.writer==bytes(16)
    for seq,nonce,lastseq,lastnonce in ((0,1,0,0),(1,0,0,0),(2,1,0,0),(1,1,1,0),(1,1,0,1)):
        refused(lambda:delivery(packet(sequence=seq,nonce=nonce),ep,PEER,4,lastseq,lastnonce))
    # Cancel is available only after full UMRC association/extent/freshness.
    for offset,associated in ((7,False),(16,False),(56,False),(72,False),
                              (96,False),(104,False),(128,False),(160,True),
                              (167,True),(176,True),(216,True),(224,True),
                              (272,True),(281,True),(283,True),(284,True),(336,True)):
        bad=bytearray(packet());bad[offset]^=255
        try:delivery(bytes(bad),ep,PEER,4,0,0)
        except WireError as error:assert (error.borrow is not None)==associated
        else:raise AssertionError('mutated complete association accepted')
    for kind in (6,7):
        control=delivery(packet(body=b'',control=kind),ep,PEER,0,0,0)
        assert control.control and control.information is None and control.body==b''
    native_service=receipt(endpoint(SERVICE),SOURCE,SERVICE,1,expected)
    assert native_service.gid==b''
    assert delivery(packet(kind=SERVICE,sequence=5),native_service,PEER,4,0,0).borrow.sequence==5
    # Complete generated32 defaults/fields, raw strings, root identities.
    from roswasm.generated_control import bounded_records as records, control_services
    names=[name for name in vars(records) if name.startswith('Ros2') and not name.endswith('Codec')]
    classes=[getattr(records,name) for name in names if isinstance(getattr(records,name),type) and hasattr(getattr(records,name),'CODEC_IDENTITY')]
    assert len(classes)==32
    for cls in classes:
        value=cls();encoded=value.encode();decoded=cls.decode(encoded)
        assert decoded==value
        try:cls.decode(encoded+b'\0')
        except ValueError:pass
        else:raise AssertionError('trailing record byte accepted')
    assert len([name for name in vars(control_services) if name.startswith('Ros2')])==7
    source=records.Ros2Type_description_interfacesMsgTypeSource()
    source.raw_file_contents=b'\0\xff\xfe'
    assert type(source.raw_file_contents)is bytes and type(source.decode(source.encode()).raw_file_contents)is bytes
    # Full ParameterValue fields remain independent; no union projection.
    value=records.Ros2Rcl_interfacesMsgParameterValue()
    value.type=255;value.bool_value=True;value.integer_value=-(1<<63);value.double_value=-0.0
    value.string_value='机器人';value.byte_array_value=[0,255];value.bool_array_value=[True,False]
    value.integer_array_value=[-(1<<63),(1<<63)-1];value.double_array_value=[-0.0,2.5]
    value.string_array_value=['','α']
    assert value.decode(value.encode())==value
    print('OWNED_SERVICE_COMPLETE_VALUE_MODELS_NOT_NATIVE_PROOF')
    return 0
