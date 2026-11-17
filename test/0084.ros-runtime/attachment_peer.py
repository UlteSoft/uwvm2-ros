"""Test-only kernel-credential and TLS transport. Only the external peer loads the fixture library."""
import ctypes
import fcntl
import os
import time
from cryptography.hazmat.primitives.asymmetric.ed25519 import Ed25519PrivateKey
from cryptography.hazmat.primitives.serialization import Encoding,NoEncryption,PrivateFormat,PublicFormat

def issue(owned):
    a,b=Ed25519PrivateKey.generate(),Ed25519PrivateKey.generate()
    fds=[]
    for local,remote in ((a,b),(b,a)):
        fd=os.memfd_create("test-only-peer-identity",os.MFD_ALLOW_SEALING|os.MFD_CLOEXEC)
        owned.callback(os.close,fd)
        assert os.write(fd,local.private_bytes(Encoding.Raw,PrivateFormat.Raw,NoEncryption())+remote.public_key().public_bytes(Encoding.Raw,PublicFormat.Raw))==64
        fcntl.fcntl(fd,fcntl.F_ADD_SEALS,fcntl.F_SEAL_WRITE|fcntl.F_SEAL_GROW|fcntl.F_SEAL_SHRINK|fcntl.F_SEAL_SEAL)
        fds.append(fd)
    return fds

class SecurePeer:
    def __init__(self, library,fd,key,pid,purpose,binding,frame):
        lib=ctypes.CDLL(str(library))
        ptr,size=ctypes.c_void_p,ctypes.c_size_t
        lib.wros_test_accept.argtypes=[ctypes.c_int,ctypes.c_int,ctypes.c_uint32,ctypes.c_uint,ptr,size]
        lib.wros_test_accept.restype=ptr
        lib.wros_test_receive.argtypes=[ptr,ptr,size,ctypes.POINTER(size)]
        lib.wros_test_send.argtypes=[ptr,ptr,size]
        lib.wros_test_close.argtypes=[ptr];lib.wros_test_close.restype=None
        self.lib=lib;self.frame=frame
        self.handle=lib.wros_test_accept(fd,key,pid,purpose,binding,frame)
        if not self.handle:raise ValueError("secure test peer registration rejected")
    def wait(self,fn):
        end=time.monotonic()+10
        while True:
            e=fn()
            if e==0:return
            if e!=11 or time.monotonic()>=end:raise ConnectionError("secure peer status "+str(e))
            time.sleep(.0001)
    def recv(self):
        buf=ctypes.create_string_buffer(self.frame);size=ctypes.c_size_t()
        self.wait(lambda:self.lib.wros_test_receive(self.handle,buf,self.frame,ctypes.byref(size)))
        return buf.raw[:size.value]
    def sendall(self,data):self.wait(lambda:self.lib.wros_test_send(self.handle,data,len(data)))
    def close(self):
        if self.handle:self.lib.wros_test_close(self.handle);self.handle=None
