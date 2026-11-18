/** @file @brief Bounded native XRCE client for a private ROS Int32 request/reply wire test. */
#include <uxr/client/client.h>
#include <ucdr/microcdr.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define TYPE "<dataType>std_msgs::msg::dds_::Int32_</dataType>"
#define TOPIC(name) "<topic><name>" name "</name>" TYPE "</topic>"
#define ENDPOINT(name) "<topic><kind>NO_KEY</kind><name>" name "</name>" TYPE "</topic>"
#define QOS "<qos><reliability><kind>RELIABLE</kind></reliability></qos>"
#define REQUEST "rt/wros_dds_request"
#define REPLY "rt/wros_dds_reply"

struct observation { int32_t expected; bool received; bool mismatch; };

static void on_reply(uxrSession* session, uxrObjectId object, uint16_t request,
                     uxrStreamId stream, ucdrBuffer* sample, uint16_t length, void* context)
{
    (void)session; (void)request; (void)stream;
    struct observation* seen = context;
    int32_t value = 0;
    if (object.id != 1 || object.type != UXR_DATAREADER_ID || length != 4 ||
        !ucdr_deserialize_int32_t(sample, &value) || value != seen->expected)
        seen->mismatch = true;
    else
        seen->received = true;
}

int main(int argc, char** argv)
{
    if (argc != 2) return 2;
    char* end = NULL;
    long number = strtol(argv[1], &end, 10);
    if (*end || number < 1 || number > (1L << 29)) return 2;
    int32_t token = (int32_t)number;
    struct observation seen = {token + 1, false, false};
    uxrUDPTransport transport;
    if (!uxr_init_udp_transport(&transport, UXR_IPv4, "127.0.0.1", "22019")) return 3;
    uxrSession session;
    uxr_init_session(&session, &transport.comm, 0x57524f53);
    uxr_set_topic_callback(&session, on_reply, &seen);
    if (!uxr_create_session(&session)) return 4;
    uint8_t outgoing[8 * UXR_CONFIG_UDP_TRANSPORT_MTU], incoming[sizeof(outgoing)];
    uxrStreamId tx = uxr_create_output_reliable_stream(&session, outgoing, sizeof(outgoing), 8);
    uxrStreamId rx = uxr_create_input_reliable_stream(&session, incoming, sizeof(incoming), 8);
    uxrObjectId participant = uxr_object_id(1, UXR_PARTICIPANT_ID);
    uxrObjectId pub = uxr_object_id(1, UXR_PUBLISHER_ID), sub = uxr_object_id(1, UXR_SUBSCRIBER_ID);
    uxrObjectId writer = uxr_object_id(1, UXR_DATAWRITER_ID), reader = uxr_object_id(1, UXR_DATAREADER_ID);
    uint16_t requests[7];
    requests[0] = uxr_buffer_create_participant_xml(&session,tx,participant,61,
        "<dds><participant><rtps><name>wros_xrce_wire</name></rtps></participant></dds>",UXR_REPLACE);
    requests[1] = uxr_buffer_create_topic_xml(&session,tx,uxr_object_id(1,UXR_TOPIC_ID),participant,
        "<dds>" TOPIC(REQUEST) "</dds>",UXR_REPLACE);
    requests[2] = uxr_buffer_create_topic_xml(&session,tx,uxr_object_id(2,UXR_TOPIC_ID),participant,
        "<dds>" TOPIC(REPLY) "</dds>",UXR_REPLACE);
    requests[3] = uxr_buffer_create_publisher_xml(&session,tx,pub,participant,"",UXR_REPLACE);
    requests[4] = uxr_buffer_create_subscriber_xml(&session,tx,sub,participant,"",UXR_REPLACE);
    requests[5] = uxr_buffer_create_datawriter_xml(&session,tx,writer,pub,
        "<dds><data_writer>" ENDPOINT(REQUEST) QOS "</data_writer></dds>",UXR_REPLACE);
    requests[6] = uxr_buffer_create_datareader_xml(&session,tx,reader,sub,
        "<dds><data_reader>" ENDPOINT(REPLY) QOS "</data_reader></dds>",UXR_REPLACE);
    uint8_t status[7] = {0};
    if (!uxr_run_session_until_all_status(&session, 3000, requests, status, 7)) {
        for (unsigned i=0;i<7;i++) fprintf(stderr,"entity %u status %u\n",i,(unsigned)status[i]);
        return 5;
    }
    uxrDeliveryControl limits = {0};
    limits.max_samples = 64;
    uxr_buffer_request_data(&session,tx,reader,rx,&limits);
    for (unsigned n=0;n<64 && !seen.received && !seen.mismatch;n++) {
        ucdrBuffer payload;
        if (uxr_prepare_output_stream(&session,tx,writer,&payload,4) == UXR_INVALID_REQUEST_ID ||
            !ucdr_serialize_int32_t(&payload,token)) return 6;
        uxr_run_session_time(&session,120);
    }
    uxr_delete_session(&session);
    uxr_close_udp_transport(&transport);
    if (!seen.received || seen.mismatch) return 7;
    printf("XRCE sent %ld and received %ld\n",number,number+1);
    return 0;
}
