/** @file @brief Echo a bounded integer on native ROS-mapped DDS topics. */
#include "Int32.h"
#include <dds/dds.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    if (argc != 3) return 1;
    unsigned const domain = (unsigned)strtoul(argv[1], NULL, 10);
    int32_t const challenge = (int32_t)strtol(argv[2], NULL, 10);
    dds_entity_t participant = dds_create_participant(domain, NULL, NULL);
    if (participant < 0) return 2;
    dds_entity_t request = dds_create_topic(participant, &std_msgs_msg_dds__Int32__desc,
                                            "rt/wros_dds_request", NULL, NULL);
    dds_entity_t reply = dds_create_topic(participant, &std_msgs_msg_dds__Int32__desc,
                                          "rt/wros_dds_reply", NULL, NULL);
    dds_qos_t *qos = dds_create_qos();
    dds_qset_reliability(qos, DDS_RELIABILITY_RELIABLE, DDS_SECS(1));
    dds_entity_t reader = dds_create_reader(participant, request, qos, NULL);
    dds_entity_t writer = dds_create_writer(participant, reply, qos, NULL);
    dds_delete_qos(qos);
    if (reader < 0 || writer < 0) return 3;
    std_msgs_msg_dds__Int32_ sample = {0};
    void *samples[] = {&sample};
    dds_sample_info_t info;
    dds_time_t end = dds_time() + DDS_SECS(12);
    unsigned answers = 0;
    while (dds_time() < end) {
        int n = dds_take(reader, samples, &info, 1, 1);
        if (n < 0) return 4;
        if (n == 1 && info.valid_data) {
            if (sample.data != challenge) return 5;
            sample.data = challenge + 1;
            if (dds_write(writer, &sample) < 0) return 6;
            if (!answers++) end = dds_time() + DDS_SECS(2);
        }
        dds_sleepfor(DDS_MSECS(5));
    }
    if (dds_delete(participant) < 0 || !answers) return 7;
    puts("Cyclone DDS received challenge and sent reply");
    return 0;
}
