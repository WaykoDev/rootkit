#!/bin/sh

apk add openrc
apk add util-linux
apk add build-base
apk add busybox-openrc

ln -s agetty /etc/init.d/agetty.ttyS0
echo ttyS0 > /etc/securetty
rc-update add agetty.ttyS0 default
rc-update add root default

echo "root:root" | chpasswd
adduser user -D
echo "user:user" | chpasswd

rc-update add devfs boot
rc-update add procfs boot
rc-update add sysfs boot

rc-update add syslog boot
rc-update add networking boot
rc-update add rootkit default

for d in bin etc lib root sbin usr; do tar c "/$d" | tar x -C /my-rootfs; done
for dir in dev proc run sys var; do mkdir /my-rootfs/${dir}; done