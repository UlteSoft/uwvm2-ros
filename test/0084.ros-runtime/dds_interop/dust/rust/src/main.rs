use dust_dds::{
    domain::domain_participant_factory::DomainParticipantFactory,
    infrastructure::{
        error::DdsError,
        listener::NO_LISTENER,
        qos::{DataReaderQos, DataWriterQos, QosKind},
        qos_policy::{
            DataRepresentationQosPolicy, ReliabilityQosPolicy, ReliabilityQosPolicyKind,
            XCDR_DATA_REPRESENTATION
        },
        sample_info::{ANY_INSTANCE_STATE, ANY_SAMPLE_STATE, ANY_VIEW_STATE},
        status::NO_STATUS,
        time::{Duration, DurationKind},
        type_support::DdsType,
    },
};
use std::{thread, time::{Duration as Delay, Instant}};

#[derive(Debug, DdsType)]
#[dust_dds(name = "std_msgs::msg::dds_::Int32_", extensibility = "final")]
struct Int32 {
    data: i32
}

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let args: Vec<_> = std::env::args().collect();
    assert_eq!(args.len(), 4);
    let echo = match args[1].as_str() {
        "echo" => true,
        "request" => false,
        _ => panic!("unknown role")
    };
    let domain = args[2].parse::<i32>()?;
    let token = args[3].parse::<i32>()?;
    assert!((0..100).contains(&domain) && (1..(1 << 29)).contains(&token));

    let factory = DomainParticipantFactory::get_instance();
    factory.get_mut_transport().set_interface_name(Some("eth0".to_owned()));
    let participant = factory.create_participant(domain, QosKind::Default, NO_LISTENER, NO_STATUS)?;
    let request = participant.create_topic::<Int32>(
        "rt/wros_dds_request", "std_msgs::msg::dds_::Int32_", QosKind::Default, NO_LISTENER, NO_STATUS
    )?;
    let reply = participant.create_topic::<Int32>(
        "rt/wros_dds_reply", "std_msgs::msg::dds_::Int32_", QosKind::Default, NO_LISTENER, NO_STATUS
    )?;
    let publisher = participant.create_publisher(QosKind::Default, NO_LISTENER, NO_STATUS)?;
    let subscriber = participant.create_subscriber(QosKind::Default, NO_LISTENER, NO_STATUS)?;
    let reliability = ReliabilityQosPolicy {
        kind: ReliabilityQosPolicyKind::Reliable,
        max_blocking_time: DurationKind::Finite(Duration::new(1, 0))
    };
    let representation = DataRepresentationQosPolicy { value: vec![XCDR_DATA_REPRESENTATION] };
    let writer = publisher.create_datawriter(
        if echo { &reply } else { &request },
        QosKind::Specific(DataWriterQos {
            reliability: reliability.clone(),
            representation: representation.clone(),
            ..Default::default()
        }),
        NO_LISTENER, NO_STATUS
    )?;
    let reader = subscriber.create_datareader::<Int32>(
        if echo { &request } else { &reply },
        QosKind::Specific(DataReaderQos { reliability, representation, ..Default::default() }),
        NO_LISTENER, NO_STATUS
    )?;

    let (mut reads, mut writes, mut mr, mut mw, mut participants) = (0, 0, 0, 0, 0);
    let mut end = Instant::now() + Delay::from_secs(12);
    while Instant::now() < end {
        participants = participants.max(participant.get_discovered_participants()?.len());
        mr = mr.max(writer.get_matched_subscriptions()?.len());
        mw = mw.max(reader.get_matched_publications()?.len());
        if !echo && mr > 0 {
            writer.write(Int32 { data: token }, None)?;
            writes += 1;
        }
        match reader.take(8, ANY_SAMPLE_STATE, ANY_VIEW_STATE, ANY_INSTANCE_STATE) {
            Ok(batch) => for sample in batch {
                if let Some(v) = sample.data {
                    if v.data != token + i32::from(!echo) {
                        return Err("wrong challenge".into());
                    }
                    if reads == 0 && echo {
                        end = Instant::now() + Delay::from_secs(2);
                    }
                    reads += 1;
                    if echo {
                        writer.write(Int32 { data: token + 1 }, None)?;
                        writes += 1;
                    }
                }
            },
            Err(DdsError::NoData) => (),
            Err(e) => return Err(e.into()),
        }
        if !echo && reads > 0 {
            break;
        }
        thread::sleep(Delay::from_millis(40));
    }

    println!(
        r#"{{"role":"{}","challenge":{},"received":{},"writes":{},"matched_readers":{},"matched_writers":{},"participants":{}}}"#,
        args[1], token, reads, writes, mr, mw, participants
    );
    if reads == 0 || writes == 0 {
        std::process::exit(3);
    }
    Ok(())
}
