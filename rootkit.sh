#!/bin/bash

print_help () {
    cat << EOF 
usage: rootkit {create,start,update,mount,umount}

options:
    -d | --disk file : The target image
    -s | --size size : The size of the disk (only for create) (default 450M)
    -l | --linux path : The path to the linux kernel
    -h | --help : Show this help
EOF
}   

LINUX_PATH=./linux-6.11.5/arch/x86/boot/bzImage
DISK_FILE="disk.img"
DISK_SIZE="450M"

TMP_FOLDER=/tmp/my-rootfs
FS_FOLDER=./fs
SETUP_FOLDER=./setup

mount_rootkit () {
    sudo losetup -Pf $DISK_FILE
    LOOP=$(losetup -l | grep disk.img | cut -d '/' -f3 | cut -d ' ' -f1)
    sudo mkdir -p $TMP_FOLDER
    sudo mount /dev/${LOOP}p1 $TMP_FOLDER
}

umount_rootkit () {
    LOOP=$(losetup -l | grep disk.img | cut -d '/' -f3 | cut -d ' ' -f1)
    LOOP_COUNT=$(losetup -l | grep disk.img | cut -d '/' -f3 | cut -d ' ' -f1 | wc -l)
    if [ $LOOP_COUNT -ne 1 ]; then
        echo "Error : Failed to detach the loop"
        exit
    fi

    sudo umount $TMP_FOLDER
    sudo losetup -d /dev/$LOOP
}

update_rootkit () {
    mount_rootkit
    sudo cp -r $FS_FOLDER/* $TMP_FOLDER/
    umount_rootkit
}

create_rootkit () {

    truncate -s $DISK_SIZE $DISK_FILE
    /sbin/parted -s $DISK_FILE mktable msdos
    /sbin/parted -s $DISK_FILE mkpart primary ext4 1 "100%"
    /sbin/parted -s $DISK_FILE set 1 boot on
    
    sudo losetup -Pf $DISK_FILE
    echo -e "[+] Creating disk $DISK_FILE with size $DISK_SIZE\n"

    LOOP=$(losetup -l | grep disk.img | cut -d '/' -f3 | cut -d ' ' -f1)
    LOOP_COUNT=$(losetup -l | grep disk.img | cut -d '/' -f3 | cut -d ' ' -f1 | wc -l)
    if [ $LOOP_COUNT -ne 1 ]; then
        echo "Error : Failed to attach the loop"
        exit
    fi

    sudo mkfs.ext4 /dev/${LOOP}p1
    mkdir -p $TMP_FOLDER
    sudo mount /dev/${LOOP}p1 $TMP_FOLDER

    sudo docker build ./setup -t rf
    sudo docker run --rm -v /tmp/my-rootfs:/my-rootfs rootkit-vm /init.sh

    sudo mkdir -p $TMP_FOLDER/boot/grub
    sudo cp $LINUX_PATH $TMP_FOLDER/boot/vmlinuz
    sudo grub-install --directory=/usr/lib/grub/i386-pc --boot-directory=$TMP_FOLDER/boot /dev/$LOOP
    
    sudo mkdir $TMP_FOLDER/home

    sudo umount $TMP_FOLDER
    sudo losetup -d /dev/$LOOP

    update_rootkit
}

start_rootkit () {
    qemu-system-x86_64 -hda $DISK_FILE -nographic
    exit 0
}

i=$(($#-2))
while [ $i -ge 0 ];
do        
    case ${BASH_ARGV[$i]} in
        -h|--help)
            print_help
            exit 0
            ;;
        -d|--disk)
            # TODO : check next value
            i=$((i-1))
            DISK_FILE=${BASH_ARGV[$i]}
            ;;
        -s|--size)
            # TODO : check next value
            i=$((i-1))
            DISK_SIZE=${BASH_ARGV[$i]}
            ;;
        -l|--linux)
            # TODO : check next value
            i=$((i-1))
            LINUX_PATH=${BASH_ARGV[$i]}
            ;;
        *)
            echo "Unknown argument ${BASH_ARGV[$i]}"
            exit 1
        ;;
    esac
    i=$((i-1))
done

if [ "$#" -lt 1 ] ||  [ $1 = "-h" ]  ||  [ $1 = "--help" ]
then
    print_help
    exit 1
fi

if [ $1 = "create" ]; then
    create_rootkit
    echo "[+] Disk created!"
elif [ $1 = "start" ] || [ $1 = "run" ]; then
    echo "[+] Rootkit launch!"
    start_rootkit
elif [ $1 = "update" ]; then
    update_rootkit
    echo "[+] Disk updated!"
elif [ $1 = "mount" ]; then
    mount_rootkit
    echo "[+] Disk successfully mount!"
elif [ $1 = "umount" ]; then
    umount_rootkit
    echo "[+] Disk successfully umount!"
fi