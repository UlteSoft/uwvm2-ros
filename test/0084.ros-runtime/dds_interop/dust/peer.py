"""One independent Dust DDS endpoint for an explicitly selected Int32 topic pair."""
import argparse
from dataclasses import dataclass
import json
import time

def main():
    import dust_dds as d
    args = argparse.ArgumentParser()
    args.add_argument("role", choices=("echo", "request"))
    args.add_argument("domain", type=int)
    args.add_argument("challenge", type=int)
    a = args.parse_args()
    assert 0 <= a.domain < 100 and 0 < a.challenge < 2**29
    @dataclass
    class Int32:
        data: d.TypeKind.int32
    Int32.__name__ = "std_msgs::msg::dds_::Int32_"
    p = d.DomainParticipantFactory.get_instance().create_participant(domain_id=a.domain)
    request = p.create_topic("rt/wros_dds_request", Int32)
    reply = p.create_topic("rt/wros_dds_reply", Int32)
    assert request.get_type_name() == reply.get_type_name() == Int32.__name__
    qos = d.ReliabilityQosPolicy(d.ReliabilityQosPolicyKind.Reliable, d.DurationKind.Finite(d.Duration(0, 100000000)))
    echo = a.role == "echo"
    publisher, subscriber = p.create_publisher(), p.create_subscriber()
    writer = publisher.create_datawriter(reply if echo else request, qos=d.DataWriterQos(reliability=qos))
    reader = subscriber.create_datareader(request if echo else reply, qos=d.DataReaderQos(reliability=qos))
    until = time.monotonic() + 12
    reads, writes, matched_readers, matched_writers = 0, 0, 0, 0
    while time.monotonic() < until:
        matched_writers = max(matched_writers, len(reader.get_matched_publications()))
        matched_readers = max(matched_readers, len(writer.get_matched_subscriptions()))
        if not echo:
            writer.write(Int32(a.challenge))
            writes += 1
        for sample in reader.take(max_samples=8):
            value = sample.get_data()
            if value is None:
                continue
            if value.data != (a.challenge if echo else a.challenge + 1):
                raise ValueError("unexpected independent peer payload")
            if not reads and echo:
                until = time.monotonic() + 2
            reads += 1
            if echo:
                writer.write(Int32(a.challenge + 1))
                writes += 1
        if reads and not echo:
            break
        time.sleep(.04)
    print(json.dumps(dict(role=a.role,challenge=a.challenge,received=reads,writes=writes,
        matched_readers=matched_readers,matched_writers=matched_writers)),flush=True)
    return 0 if reads and writes else 3

if __name__ == "__main__":
    raise SystemExit(main())
