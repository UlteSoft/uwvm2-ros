/* Authored CPU layout regressions; no native ROS/GPU or Wasm transport credit. */
#include "cpu_sensor_contract.h"
#include <stdio.h>
#include <string.h>

static unsigned cases_run;
static int check(int actual, int expected, unsigned line) {
    ++cases_run;
    if (actual == expected)
        return 0;
    fprintf(stderr, "line %u: got %d, expected %d\n", line, actual, expected);
    return 1;
}
#define CHECK(value, expected)                                                                     \
    do {                                                                                           \
        if (check((value), (expected), __LINE__))                                                  \
            return 1;                                                                              \
    } while (0)

int main(void) {
    unsigned char image_bytes[16] = {0};
    unsigned char cloud_bytes[32] = {0};
    unsigned char invalid_utf8[] = {0xed, 0xa0, 0x80};
    unsigned char valid_utf8[] = {0xf0, 0x9f, 0xa4, 0x96};
    struct uwvm_nvidia_cpu_image image;
    struct uwvm_nvidia_cpu_pointcloud2 cloud;
    struct uwvm_nvidia_pointfield field;
    struct uwvm_nvidia_text_view text;
    memset(&image, 0, sizeof image);
    memset(&cloud, 0, sizeof cloud);
    memset(&field, 0, sizeof field);
    image.width = 2;
    image.height = 2;
    image.step = 8;
    image.encoding.data = (const unsigned char *)"rgb8";
    image.encoding.size = 4;
    image.data.data = image_bytes;
    image.data.size = sizeof image_bytes;
    CHECK(uwvm_nvidia_validate_apriltag_cpu_image(&image, 16, 64), 0);
    image.is_bigendian = 1;
    CHECK(uwvm_nvidia_validate_apriltag_cpu_image(&image, 16, 64), 0);
    image.is_bigendian = 2;
    CHECK(uwvm_nvidia_validate_apriltag_cpu_image(&image, 16, 64), 22);
    image.is_bigendian = 0;
    CHECK(uwvm_nvidia_validate_apriltag_cpu_image(&image, 15, 64), 90);
    image.data.size = 15;
    CHECK(uwvm_nvidia_validate_apriltag_cpu_image(&image, 16, 64), 22);
    image.data.size = 16;
    image.step = 5;
    CHECK(uwvm_nvidia_validate_apriltag_cpu_image(&image, 16, 64), 22);
    image.step = 8;
    image.header.stamp.nanosec = 1000000000u;
    CHECK(uwvm_nvidia_validate_apriltag_cpu_image(&image, 16, 64), 22);
    image.header.stamp.nanosec = 999999999u;
    image.header.stamp.sec = -1;
    CHECK(uwvm_nvidia_validate_apriltag_cpu_image(&image, 16, 64), 0);
    image.encoding.data = (const unsigned char *)"16UC1";
    image.encoding.size = 5;
    CHECK(uwvm_nvidia_validate_apriltag_cpu_image(&image, 16, 64), 95);
    image.encoding.data = (const unsigned char *)"mono8";
    CHECK(uwvm_nvidia_validate_apriltag_cpu_image(&image, 16, 64), 0);
    image.width = UINT32_MAX;
    image.height = UINT32_MAX;
    image.step = UINT32_MAX;
    CHECK(uwvm_nvidia_validate_apriltag_cpu_image(&image, 16, 64), 90);

    text.data = invalid_utf8;
    text.size = sizeof invalid_utf8;
    CHECK(uwvm_nvidia_sensor_text(text, 64), 22);
    text.data = valid_utf8;
    text.size = sizeof valid_utf8;
    CHECK(uwvm_nvidia_sensor_text(text, 64), 0);
    CHECK(uwvm_nvidia_sensor_text(text, 3), 90);
    text.data = (const unsigned char *)"a\0b";
    text.size = 3;
    CHECK(uwvm_nvidia_sensor_text(text, 64), 22);

    cloud.width = 2;
    cloud.height = 1;
    cloud.point_step = 12;
    cloud.row_step = 32;
    cloud.data.data = cloud_bytes;
    cloud.data.size = sizeof cloud_bytes;
    cloud.fields = &field;
    cloud.fields_size = 1;
    field.name.data = (const unsigned char *)"position";
    field.name.size = 8;
    field.datatype = UWVM_NVIDIA_POINTFIELD_FLOAT32;
    field.count = 3;
    CHECK(uwvm_nvidia_validate_cpu_pointcloud2(&cloud, UWVM_NVIDIA_POINTFIELD_LEGACY_8, 32, 16, 64),
          0);
    cloud.is_bigendian = 1;
    CHECK(
        uwvm_nvidia_validate_cpu_pointcloud2(&cloud, UWVM_NVIDIA_POINTFIELD_LYRICAL_11, 32, 16, 64),
        0);
    field.datatype = UWVM_NVIDIA_POINTFIELD_INT64;
    field.count = 1;
    CHECK(uwvm_nvidia_validate_cpu_pointcloud2(&cloud, UWVM_NVIDIA_POINTFIELD_LEGACY_8, 32, 16, 64),
          95);
    CHECK(
        uwvm_nvidia_validate_cpu_pointcloud2(&cloud, UWVM_NVIDIA_POINTFIELD_LYRICAL_11, 32, 16, 64),
        0);
    field.datatype = UWVM_NVIDIA_POINTFIELD_UINT64;
    CHECK(
        uwvm_nvidia_validate_cpu_pointcloud2(&cloud, UWVM_NVIDIA_POINTFIELD_LYRICAL_11, 32, 16, 64),
        0);
    field.datatype = UWVM_NVIDIA_POINTFIELD_BOOL;
    CHECK(
        uwvm_nvidia_validate_cpu_pointcloud2(&cloud, UWVM_NVIDIA_POINTFIELD_LYRICAL_11, 32, 16, 64),
        0);
    field.offset = 12;
    CHECK(
        uwvm_nvidia_validate_cpu_pointcloud2(&cloud, UWVM_NVIDIA_POINTFIELD_LYRICAL_11, 32, 16, 64),
        22);
    field.offset = 0;
    field.count = UINT32_MAX;
    CHECK(
        uwvm_nvidia_validate_cpu_pointcloud2(&cloud, UWVM_NVIDIA_POINTFIELD_LYRICAL_11, 32, 16, 64),
        22);
    field.count = 1;
    cloud.row_step = 23;
    CHECK(
        uwvm_nvidia_validate_cpu_pointcloud2(&cloud, UWVM_NVIDIA_POINTFIELD_LYRICAL_11, 32, 16, 64),
        22);
    cloud.row_step = 32;
    CHECK(
        uwvm_nvidia_validate_cpu_pointcloud2(&cloud, UWVM_NVIDIA_POINTFIELD_LYRICAL_11, 31, 16, 64),
        90);
    CHECK(
        uwvm_nvidia_validate_cpu_pointcloud2(&cloud, UWVM_NVIDIA_POINTFIELD_LYRICAL_11, 32, 0, 64),
        90);
    cloud.fields_size = 0;
    cloud.width = 0;
    cloud.height = 0;
    cloud.row_step = 0;
    cloud.point_step = 0;
    cloud.data.size = 0;
    CHECK(
        uwvm_nvidia_validate_cpu_pointcloud2(&cloud, UWVM_NVIDIA_POINTFIELD_LYRICAL_11, 32, 16, 64),
        0);
    printf("{\"scope\":\"authored_cpu_layout\",\"checks\":%u,\"native_ros\":0,\"gpu\":0}\n",
           cases_run);
    return 0;
}
