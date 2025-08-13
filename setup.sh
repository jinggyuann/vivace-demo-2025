#!/bin/bash

IMG_NAME="randomusb.img"
EXE_SOURCE="$HOME/Desktop/RobuxGenerator.exe"
MOUNT_DIR="$HOME/randomusb"
LABEL="RANDOMUSB"
IMG_SIZE_MB=105

exec > >(tee -i $LOG_FILE)
exec 2>&1

echo "=== USB Demo Setup Started ==="
echo "Timestamp: $(date)"
echo "Image: $IMG_NAME"
echo "Mount directory: $MOUNT_DIR"
echo ""
if [ ! -f "$IMG_NAME" ]; then
    echo "[+] Creating blank image: $IMG_NAME"
    dd if=/dev/zero of=$IMG_NAME bs=1M count=$IMG_SIZE_MB
else
    echo "[✓] Image already exists: $IMG_NAME"
fi

LOOP_DEV=$(sudo losetup -Pf --show $IMG_NAME)
PARTITION="${LOOP_DEV}p1"
echo "[+] Loop device: $LOOP_DEV"

PART_EXISTS=$(ls ${LOOP_DEV}p1 2>/dev/null)
if [ -z "$PART_EXISTS" ]; then
    echo "[!] No partition found — creating FAT32 partition"
    parted $IMG_NAME --script mklabel msdos
    parted $IMG_NAME --script mkpart primary fat32 2048s 100%
    sudo losetup -d $LOOP_DEV
    LOOP_DEV=$(sudo losetup -Pf --show $IMG_NAME)
    PARTITION="${LOOP_DEV}p1"
    echo "[+] Reattached loop device: $LOOP_DEV"
    echo "[+] Formatting partition as FAT32"
    sudo mkfs.vfat -n $LABEL $PARTITION
else
    echo "[✓] Existing partition found: $PARTITION"
fi

mkdir -p $MOUNT_DIR
sudo mount -o uid=$(id -u),gid=$(id -g) $PARTITION $MOUNT_DIR

if [ -f "$EXE_SOURCE" ]; then
    cp "$EXE_SOURCE" "$MOUNT_DIR/"
    echo "[✓] Copied malware.exe to $MOUNT_DIR"
else
    echo "[!] malware.exe not found at $EXE_SOURCE"
fi

echo ""
echo "=== Setup Complete ==="
echo "You may now manually delete 'RobuxGenerator' from the mounted USB."
echo "When ready, run 'recovery.sh' to generate the forensic image."
