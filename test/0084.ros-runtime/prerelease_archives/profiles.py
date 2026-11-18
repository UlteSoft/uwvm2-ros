"""Generate the two local discovery profiles used by the historical chatter probes."""
import argparse
import ipaddress
from pathlib import Path
import xml.etree.ElementTree as ET


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument("--address", type=ipaddress.IPv4Address, required=True)
    p.add_argument("--opensplice-template", type=Path, help="ospl_sp_ddsi.xml from the verified runtime")
    p.add_argument("--fastdds-template", type=Path, help="defaults to the neighboring DDS interop fixture")
    p.add_argument("--output", type=Path, required=True)
    args = p.parse_args()
    args.output.mkdir(exist_ok=False, parents=True)
    address = str(args.address)
    if args.opensplice_template:
        ospl = ET.parse(args.opensplice_template)
        service = ospl.find("DDSI2Service")
        if service is None:
            p.error("the OpenSplice template requires a DDSI2Service")
        interface = service.find("General/NetworkInterfaceAddress")
        multicast = service.find("General/AllowMulticast")
        if interface is None or multicast is None or service.find("Discovery") is not None:
            p.error("expected the original OpenSplice single-process DDSI template")
        interface.text = address
        multicast.text = "false"
        discovery = ET.SubElement(service, "Discovery")
        ET.SubElement(discovery, "ParticipantIndex").text = "auto"
        ET.SubElement(ET.SubElement(discovery, "Peers"), "Peer", Address=address)
        ospl.write(args.output / "ospl.xml", encoding="utf-8", xml_declaration=True)
    template = args.fastdds_template or Path(__file__).resolve().parents[1] / "dds_interop/fastdds-loopback.xml"
    fast = ET.parse(template)
    namespace = "http://www.eprosima.com/XMLSchemas/fastRTPS_Profiles"
    ET.register_namespace("", namespace)
    addresses = fast.findall(".//{" + namespace + "}address")
    if len(addresses) != 3 or any(item.text != "127.0.0.1" for item in addresses):
        p.error("expected the loopback Fast DDS fixture's three addresses")
    for item in addresses:
        item.text = address
    fast.write(args.output / "fastdds.xml", encoding="utf-8", xml_declaration=True)


if __name__ == "__main__":
    main()
