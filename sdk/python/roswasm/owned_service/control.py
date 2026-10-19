"""Capture generated complete component classes and seven original root facts."""
from dataclasses import dataclass
from .owned import ServiceDefinition


@dataclass(frozen=True)
class RecordCodec:
    record: type
    codec_identity: str
    maximum: int

    def encode(self, value):
        if type(value) is not self.record:
            raise TypeError('exact full generated component required')
        return self.record.encode(value)

    def decode(self, data):
        return self.record.decode(data)


def install_services(namespace, records, services):
    if type(services) is not list or len(services) != 7 or len({row['native_type'] for row in services}) != 7:
        raise ValueError('complete seven original Service roots required')
    for row in services:
        request, response = getattr(records, row['request']), getattr(records, row['response'])
        definition = ServiceDefinition(row['native_type'], row['codec_identity'], row['native_identity'],
            RecordCodec(request, request.CODEC_IDENTITY, request.MAX_WIRE_BYTES),
            RecordCodec(response, response.CODEC_IDENTITY, response.MAX_WIRE_BYTES))
        namespace[row['symbol']] = definition
