/** Fixed Int32 domain isolation witness and test-only forwarding pair. */
#include "Int32.h"
#include <dds/dds.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum { request, reply, topic_count };
enum { jazzy, humble, domain_count };

struct domain {
    dds_entity_t participant;
    dds_entity_t topics[topic_count];
    dds_entity_t readers[topic_count];
    unsigned observed[topic_count];
};

static bool open_domain(struct domain *state, unsigned id, const dds_qos_t *qos)
{
    const char *names[topic_count] = {"rt/wros_dds_request", "rt/wros_dds_reply"};
    state->participant = dds_create_participant(id, NULL, NULL);
    if (state->participant < 0) return false;
    for (int i = 0; i < topic_count; ++i) {
        state->topics[i] = dds_create_topic(state->participant, &std_msgs_msg_dds__Int32__desc,
                                            names[i], NULL, NULL);
        if (state->topics[i] < 0) return false;
        state->readers[i] = dds_create_reader(state->participant, state->topics[i], qos, NULL);
        if (state->readers[i] < 0) return false;
    }
    return true;
}

int main(int argc, char **argv)
{
    if (argc != 3) return 1;
    bool const forwarding = strcmp(argv[1], "forward") == 0;
    char *end = NULL;
    long const challenge = strtol(argv[2], &end, 10);
    if ((!forwarding && strcmp(argv[1], "observe") != 0) || *end || challenge < 1 || challenge > (1 << 29)) return 1;

    struct domain domains[domain_count] = {0};
    dds_qos_t *qos = dds_create_qos();
    if (!qos) return 2;
    dds_qset_reliability(qos, DDS_RELIABILITY_RELIABLE, DDS_SECS(1));
    if (!open_domain(&domains[jazzy], 61, qos) || !open_domain(&domains[humble], 62, qos)) return 2;
    dds_entity_t writers[topic_count] = {0};
    if (forwarding) {
        writers[request] = dds_create_writer(domains[humble].participant, domains[humble].topics[request], qos, NULL);
        writers[reply] = dds_create_writer(domains[jazzy].participant, domains[jazzy].topics[reply], qos, NULL);
        if (writers[request] < 0 || writers[reply] < 0) return 2;
    }
    dds_delete_qos(qos);
    puts("ready");
    fflush(stdout);
    unsigned writes[topic_count] = {0};
    dds_time_t deadline = dds_time() + DDS_SECS(15);
    while (dds_time() < deadline) {
        for (int d = 0; d < domain_count; ++d) {
            for (int t = 0; t < topic_count; ++t) {
                std_msgs_msg_dds__Int32_ sample = {0};
                void *buffer[] = {&sample};
                dds_sample_info_t info;
                int count = dds_take(domains[d].readers[t], buffer, &info, 1, 1);
                if (count < 0) return 3;
                if (count == 0 || !info.valid_data) continue;
                if (sample.data != challenge + (t == reply)) return 4;
                ++domains[d].observed[t];
                if (forwarding && ((d == jazzy && t == request) || (d == humble && t == reply))) {
                    if (dds_write(writers[t], &sample) < 0) return 5;
                    ++writes[t];
                }
            }
        }
        dds_sleepfor(DDS_MSECS(2));
    }
    for (int d = 0; d < domain_count; ++d) {
        if (dds_delete(domains[d].participant) < 0) return 6;
    }
    printf("{\"request61\":%u,\"request62\":%u,\"reply62\":%u,\"reply61\":%u,"
           "\"forwardWrites\":%u,\"returnWrites\":%u}\n",
           domains[jazzy].observed[request], domains[humble].observed[request],
           domains[humble].observed[reply], domains[jazzy].observed[reply], writes[request], writes[reply]);
    return 0;
}
