/**
 * @file main.mjs
 * @brief Exercise both generated UTF-8 codec profiles in the actual JS guest.
 * @details Synthetic local records are emitter tests, never native type or ROS
 * registration evidence. The expected wire bytes are independent of the codec.
 */
import {Ros2TestMsgTextCodec as bounded} from "kuka/js/ros2/kuka-ros2-complete/generated_bounded.mjs";
import {Ros2TestMsgTextCodec as control, ControlCodecError} from "kuka/js/ros2/kuka-ros2-complete/generated_control.mjs";

/** @brief Fail the guest immediately when a semantic witness disagrees. */
function requireValue(condition)
{
    if(!condition)
        throw new Error("portable UTF-8 codec regression");
}

/** @brief Require the selected profile's exact public error category. */
function refused(action, profile)
{
    let error;
    try
    {
        action();
    }
    catch(value)
    {
        error = value;
    }
    requireValue(profile === control ? error instanceof ControlCodecError && error.code === 22 : error instanceof TypeError);
}

/** @brief Encode an independently supplied UTF-8 payload length in little endian. */
function wire(payload)
{
    const bytes = new Uint8Array(4 + payload.length);
    new DataView(bytes.buffer).setUint32(0, payload.length, true);
    bytes.set(payload, 4);
    return bytes;
}

for(const codec of [bounded, control])
{
    const positives = [
        ["", []],
        ["plain", [112, 108, 97, 105, 110]],
        ["\ufeff", [239, 187, 191]],
        ["\u00e9\u4e2d\ud83d\ude00", [195, 169, 228, 184, 173, 240, 159, 152, 128]],
        ["x".repeat(32), Array(32).fill(120)],
        ["\ud83d\ude00".repeat(8), Array.from({length: 8}, () => [240, 159, 152, 128]).flat()]
    ];
    for(const [text, payload] of positives)
    {
        const expected = wire(payload);
        const encoded = codec.encode({text});
        requireValue(encoded.length === expected.length);
        requireValue(encoded.every((byte, index) => byte === expected[index]));
        requireValue(codec.decode(expected).text === text);
        requireValue(codec.valid(expected));
    }
    for(const text of ["\0", "\ud800", "\udc00", "\ud800x", "x".repeat(33), "\ud83d\ude00".repeat(9)])
        refused(() => codec.encode({text}), codec);
    for(const payload of [[0], [128], [192, 128], [224, 128, 128], [237, 160, 128],
                          [240, 128, 128, 128], [244, 144, 128, 128], [226, 130], [226, 65, 172]])
    {
        const bytes = wire(payload);
        refused(() => codec.decode(bytes), codec);
        requireValue(!codec.valid(bytes));
    }
    const truncated = wire([97]);
    new DataView(truncated.buffer).setUint32(0, 2, true);
    refused(() => codec.decode(truncated), codec);
}
__uwvm.output("PASS portable-generated-utf8 profiles=2 positive=12 negative=32");
