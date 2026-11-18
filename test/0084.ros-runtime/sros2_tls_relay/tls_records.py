"""Copy the private socketpair traffic and count complete TLS records per direction."""
from collections import Counter
import hashlib
import socket
import threading


class Transport:
    def __init__(self,markers):
        self.markers=markers
        self.pairs=[socket.socketpair(),socket.socketpair()]
        self.threads=[]
        self.receipts={}

    def copy(self,index):
        source=self.pairs[index][1]
        target=self.pairs[1-index][1]
        data=bytearray()
        error=None
        try:
            while block:=source.recv(16384):
                if len(data)+len(block)>131072:
                    raise ValueError('test channel exceeded its byte budget')
                data.extend(block)
                target.sendall(block)
        except (OSError,ValueError) as exc:
            error=str(exc)
        finally:
            try: target.shutdown(socket.SHUT_WR)
            except OSError: pass
        offset=0
        kinds=Counter()
        while len(data)-offset>=5:
            kind,major,minor=data[offset:offset+3]
            length=int.from_bytes(data[offset+3:offset+5],'big')
            if kind not in (20,21,22,23) or major!=3 or minor not in (1,3) or length>16640 or offset+5+length>len(data):
                break
            kinds[str(kind)]+=1
            offset+=5+length
        self.receipts[('fast','cyclone')[index]]={'bytes':len(data),'sha256':hashlib.sha256(data).hexdigest(),
            'records':dict(kinds),'completeRecords':offset==len(data),'error':error,
            'clearMarkers':{name:data.count(value.encode()) for name,value in self.markers.items()}}

    def start(self):
        for index in range(2):
            thread=threading.Thread(target=self.copy,args=(index,),daemon=True)
            thread.start()
            self.threads.append(thread)
        return [pair[0] for pair in self.pairs]

    def finish(self):
        for pair in self.pairs: pair[0].close()
        for thread in self.threads:thread.join(timeout=4)
        for pair in self.pairs:pair[1].close()
        for thread in self.threads:thread.join(timeout=1)
        return self.receipts
