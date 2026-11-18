#!/bin/busybox sh
export PATH=/bin:/usr/bin:/usr/sbin
bb=/bin/busybox
$bb mount -t proc proc /proc
$bb mount -t sysfs sysfs /sys
$bb mount -t devtmpfs devtmpfs /dev
$bb mkdir -p /dev/shm
$bb mount -t tmpfs tmpfs /dev/shm
$bb chmod 1777 /dev/shm /tmp
$bb insmod /devices/e1000.ko
/usr/sbin/ip link set lo up
/usr/sbin/ip link set eth0 up
/usr/sbin/ip addr add 192.0.2.1/24 dev eth0
/usr/sbin/ip route add 224.0.0.0/4 dev eth0 src 192.0.2.1
/usr/sbin/ip -4 addr
/usr/sbin/ip -4 route
/usr/bin/setpriv --reuid=65534 --regid=65534 --clear-groups --bounding-set=-all --inh-caps=-all --ambient-caps=-all --no-new-privs /usr/bin/python3 -B /fixture/guest.py
$bb poweroff -f
