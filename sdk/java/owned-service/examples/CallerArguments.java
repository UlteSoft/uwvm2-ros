package org.uwvm.roswasm.examples.owned;

import org.uwvm.roswasm.*;

/** Source shared by the two finite command applications. Caller supplies all
 * aliases, actual expected QoS and poll budget. No default service names/grants. */
final class CallerArguments {
    final String call,observer,availability;final OwnedService.Qos request,response;final int polls;
    CallerArguments(String[] a,int minimum){
        if(a.length<minimum)throw new IllegalArgumentException("callAlias observerAlias availabilityAlias|- qos80hex qos80hex pollBudget appArguments...");
        call=a[0];observer=a[1];availability=a[2].equals("-")?null:a[2];request=OwnedService.Qos.fromBytes(hex(a[3],80));response=OwnedService.Qos.fromBytes(hex(a[4],80));polls=Integer.parseInt(a[5]);
        if(polls<1||polls>4096)throw new IllegalArgumentException("poll budget1..4096");
    }
    static byte[] hex(String s,int bytes){if(s.length()!=bytes*2)throw new IllegalArgumentException("exact hex extent");byte[] out=new byte[bytes];for(int i=0;i<bytes;i++){int h=Character.digit(s.charAt(i*2),16),l=Character.digit(s.charAt(i*2+1),16);if(h<0||l<0)throw new IllegalArgumentException("hex");out[i]=(byte)((h<<4)|l);}return out;}
    static void require(int n){if(n!=0)throw new RosException(new Result(n));}
    static <Q,S> void available(OwnedService.Client<Q,S> client,boolean selected){if(selected){OwnedService.Availability a=client.available(0);require(a.code);if(!a.available)throw new IllegalStateException("native retained client reports unavailable; no request sent");}}
    static <Q,S> OwnedService.Delivery<S> receive(OwnedService.Client<Q,S> client,int polls){for(int i=0;i<polls;i++){OwnedService.Delivery<S> d=client.takeResponse();if(d.code==11)continue;require(d.code);if(!d.hasValue)throw new IllegalStateException("actual control or abandoned record");return d;}throw new IllegalStateException("finite poll budget exhausted; no completion claimed");}
    static void info(OwnedService.Delivery<?> d){System.out.println("ticket="+Long.toUnsignedString(d.ticket)+" sequence="+Long.toUnsignedString(d.sequence)+" nonce="+Long.toUnsignedString(d.reservation)+" nativeSequence="+d.nativeSequence+" sourceNs="+d.sourceTimestamp+" receivedNs="+d.receivedTimestamp);}
}
