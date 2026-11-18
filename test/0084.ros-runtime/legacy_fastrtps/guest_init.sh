#!/bin/busybox sh
set -eu
export PATH=/bin:/usr/bin:/usr/sbin
/bin/busybox mount -t proc proc /proc
/bin/busybox mount -t sysfs sysfs /sys
/bin/busybox mount -t devtmpfs devtmpfs /dev
/bin/busybox mkdir -p /dev/pts /dev/shm
/bin/busybox mount -t devpts devpts /dev/pts
/bin/busybox mount -t tmpfs tmpfs /dev/shm
/usr/sbin/ip link set lo up
if test -f /devices/e1000.ko; then
    /bin/busybox insmod /devices/e1000.ko
    /usr/sbin/ip link set eth0 up
    /usr/sbin/ip address add 192.0.2.1/24 dev eth0
    /usr/sbin/ip route add 239.255.0.1/32 dev eth0 src 192.0.2.1
else
    /usr/sbin/ip address add 192.0.2.1/32 dev lo
    /usr/sbin/ip link set lo multicast on
    /usr/sbin/ip route add 239.255.0.1/32 dev lo src 192.0.2.1
fi
/usr/sbin/ip -4 address show
/usr/sbin/ip -4 route show
/usr/bin/python3 -B /fixture/test/0084.ros-runtime/legacy_fastrtps/guest.py
echo UWVM_REPORTS_BEGIN
/bin/busybox tar -C /results -czf - . | /bin/busybox base64
echo UWVM_REPORTS_END
/bin/busybox poweroff -f
