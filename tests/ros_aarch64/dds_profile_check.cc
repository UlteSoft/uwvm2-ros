#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/qos/DomainParticipantQos.hpp>
#include <fastrtps/attributes/ParticipantAttributes.h>
#include <fastrtps/xmlparser/XMLProfileManager.h>
#include <iostream>

int main(int argc,char** argv)
{
    using namespace eprosima::fastdds::dds;
    if(argc!=2) { return 2; }
    auto factory=DomainParticipantFactory::get_instance();
    DomainParticipantQos baseline,parsed;
    if(factory->get_default_participant_qos(baseline)!=ReturnCode_t::RETCODE_OK||
       factory->load_XML_profiles_file(argv[1])!=ReturnCode_t::RETCODE_OK||
       factory->get_participant_qos_from_profile("uwvm_qemu_discovery_100ms",parsed)!=ReturnCode_t::RETCODE_OK)
    { std::cerr<<"FAIL SDK FastDDS XML parser/profile/default acceptance\n";return 1; }
    auto expected=baseline;
    auto& discovery=expected.wire_protocol().builtin.discovery_config;
    discovery.leaseDuration_announcementperiod={0,100000000};
    discovery.initial_announcements.count=20;
    discovery.initial_announcements.period={0,100000000};
    eprosima::fastrtps::ParticipantAttributes loaded_default;
    eprosima::fastrtps::xmlparser::XMLProfileManager::getDefaultParticipantAttributes(loaded_default);
    auto const& default_discovery=loaded_default.rtps.builtin.discovery_config;
    if(!(parsed==expected)||default_discovery.leaseDuration_announcementperiod!=discovery.leaseDuration_announcementperiod||
       !(default_discovery.initial_announcements==discovery.initial_announcements))
    { std::cerr<<"FAIL parsed participant differs beyond discovery announcements or is not default\n";return 1; }
    auto const& old=baseline.wire_protocol().builtin.discovery_config;
    std::cout<<"{\"status\":\"PASS\",\"sdk_parser_accepted\":true,"
        "\"participant_default_applied\":true,\"equal_outside_discovery_announcements\":true,"
        "\"baseline_announcement_seconds\":"<<old.leaseDuration_announcementperiod.seconds<<','
        <<"\"baseline_announcement_nanoseconds\":"<<old.leaseDuration_announcementperiod.nanosec<<','
        <<"\"baseline_initial_count\":"<<old.initial_announcements.count<<','
        <<"\"announcement_nanoseconds\":100000000,\"initial_count\":20,"
        "\"initial_period_nanoseconds\":100000000,\"participants_created\":0}\n";
}
