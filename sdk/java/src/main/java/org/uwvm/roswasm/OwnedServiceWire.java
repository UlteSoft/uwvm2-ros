package org.uwvm.roswasm;

/** Actual byte layouts, not Java/native struct casts. Mutates only preallocated
 * private decode targets; every bounds/unsigned check precedes array indexing.
 */
final class OwnedServiceWire {
    static final int OVERHEAD=352, INPUT_MAXIMUM=4193626;
    private OwnedServiceWire(){}
    static long u(byte[] b,int at,int n){long x=0;for(int i=0;i<n;i++)x|=((long)b[at+i]&255)<<(i*8);return x;}
    static void put(byte[] b,int at,long x,int n){for(int i=0;i<n;i++)b[at+i]=(byte)(x>>>(i*8));}
    static boolean zero(byte[] b,int a,int z){for(int i=a;i<z;i++)if(b[i]!=0)return false;return true;}
    static boolean same(byte[] a,int at,byte[] b,int bt,int n){for(int i=0;i<n;i++)if(a[at+i]!=b[bt+i])return false;return true;}
    static boolean full(long handle){return (handle&0xffffffffL)!=0&&(handle>>>32)!=0;}
    static boolean qos(byte[] b,int at){
        for(int i=0;i<4;i++){long v=u(b,at+i*4,4);if(v<1||v>2)return false;}
        return !(u(b,at,4)==1&&u(b,at+16,8)==0)&&Long.compareUnsigned(u(b,at+32,8),1000000000L)<0
            &&Long.compareUnsigned(u(b,at+48,8),1000000000L)<0&&Long.compareUnsigned(u(b,at+64,8),1000000000L)<0
            &&(b[at+72]&255)<=1&&zero(b,at+73,at+80);
    }
    static final class Receipt {
        long source,instance,generation,session,incarnation;
        int role,gidBytes;
        boolean bindingValid;
        final byte[] nativeGid=new byte[32],qos=new byte[160];
    }
    static int receipt(byte[] b,int operation,int role,long source,Receipt r){
        r.bindingValid=false;
        if(b.length!=256||u(b,0,4)!=0x45524d55L||u(b,4,2)!=1||(b[6]&255)!=operation||b[7]!=2||u(b,8,4)!=256
            ||(b[12]&255)!=role||(role!=3&&role!=4)||b[13]!=2||(b[14]&255)>32||b[15]!=0||!zero(b,56,64))return 22;
        r.source=u(b,16,8);r.instance=u(b,24,8);r.generation=u(b,32,8);r.session=u(b,40,8);r.incarnation=u(b,48,8);r.role=role;r.gidBytes=b[14]&255;
        if(!full(source)||r.source!=source||r.instance==0||r.generation==0||r.session==0||r.incarnation==0)return 22;
        // This structurally actual binding is retained for once-cleanup even
        // when a later QoS/GID assertion refuses endpoint admission.
        r.bindingValid=true;
        if((role==4&&r.gidBytes!=0)||!zero(b,224+r.gidBytes,256)||!qos(b,64)||!qos(b,144))return 22;
        System.arraycopy(b,64,r.qos,0,160);System.arraycopy(b,224,r.nativeGid,0,32);return 0;
    }
    static boolean binding(byte[] b,int at,Receipt r){return b[at+7]==2&&u(b,at+16,8)==r.source&&u(b,at+24,8)==r.instance&&u(b,at+32,8)==r.generation&&u(b,at+40,8)==r.session&&u(b,at+48,8)==r.incarnation;}
    static final class Record {
        long sequence,reservation,ticket,sourceTimestamp,receivedTimestamp,nativeSequence;
        int bodyBytes,kind,controlFlags,error;
        boolean token;
        final byte[] writerGuid=new byte[16];
    }
    static int record(byte[] b,int capacity,Receipt r,long peer,int maximum,Record d){
        d.token=false;
        if(capacity<OVERHEAD||capacity>b.length||u(b,0,4)!=0x43524d55L||u(b,4,2)!=1||b[6]!=5||u(b,8,4)!=160
            ||(b[12]&255)!=r.role||b[13]!=16||!zero(b,14,16)||!zero(b,128,160)||!binding(b,0,r)||u(b,56,8)!=peer
            ||u(b,72,8)==0||u(b,80,8)==0||u(b,88,8)!=0||u(b,96,8)!=capacity||u(b,112,8)!=0||u(b,120,8)!=0)return 22;
        long payload=u(b,104,8);
        if(Long.compareUnsigned(payload,192)<0||Long.compareUnsigned(payload,capacity-160L)>0||Long.compareUnsigned(payload-192,maximum)>0)return 90;
        d.sequence=u(b,72,8);d.reservation=u(b,80,8);d.ticket=u(b,64,8);d.bodyBytes=(int)(payload-192);d.token=true;
        int a=160;
        if(u(b,a,4)!=0x494d5255L||u(b,a+4,2)!=1||u(b,a+8,4)!=192||!binding(b,a,r)||u(b,a+56,8)!=d.sequence
            ||u(b,a+64,8)!=d.ticket||u(b,a+112,8)!=d.bodyBytes||b[a+123]!=0||!zero(b,a+176,a+192))return 22;
        d.kind=b[a+6]&255;d.controlFlags=b[a+122]&255;d.error=(int)u(b,a+124,4);
        if(d.kind==6||d.kind==7){
            if(u(b,a+12,4)!=0||d.bodyBytes!=0||b[a+120]!=0||b[a+121]!=0||!zero(b,a+128,a+176)||u(b,a+72,8)!=0||u(b,a+80,8)!=0
                ||u(b,a+88,8)!=-1L||u(b,a+96,8)!=-1L||u(b,a+104,8)!=0||(d.controlFlags&~3)!=0||(d.kind==6&&d.error==0)||(d.kind==7&&(d.controlFlags&1)==0))return 22;
            d.sourceTimestamp=d.receivedTimestamp=d.nativeSequence=0;return 0;
        }
        if(b[a+121]!=16)return 95;
        if(d.kind!=(r.role==3?3:2)||u(b,a+12,4)!=35||d.ticket==0||d.error!=0||d.controlFlags!=0||b[a+120]!=0
            ||!zero(b,a+128,a+160)||u(b,a+88,8)!=-1L||u(b,a+96,8)!=-1L)return 22;
        d.sourceTimestamp=u(b,a+72,8);d.receivedTimestamp=u(b,a+80,8);d.nativeSequence=u(b,a+104,8);
        System.arraycopy(b,a+160,d.writerGuid,0,16);return 0;
    }
}
