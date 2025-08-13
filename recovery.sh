#!/bin/bash

IMG_NAME="randomusb.img"
RECOVERY_IMG="recovery.img"
MOUNT_DIR="$HOME/randomusb"

exec > >(tee -i $LOG_FILE)
exec 2>&1

echo "=== Recovery Image Creation Started ==="
echo "Timestamp: $(date)"
echo "Source image: $IMG_NAME"
echo "Recovery image: $RECOVERY_IMG"
echo "Mount directory: $MOUNT_DIR"
echo ""

echo "[+] Unmounting USB image..."
sudo umount $MOUNT_DIR

LOOP_DEV=$(losetup -j $IMG_NAME | cut -d: -f1)
if [ -n "$LOOP_DEV" ]; then
    echo "[+] Detaching loop device: $LOOP_DEV"
    sudo losetup -d $LOOP_DEV
else
    echo "[!] No loop device found for $IMG_NAME"
fi

echo "[+] Creating forensic image: $RECOVERY_IMG"
dd if=$IMG_NAME of=$RECOVERY_IMG bs=4M status=progress

echo ""
echo "=== Recovery Complete ==="
echo "Recovery image created: $RECOVERY_IMG"
echo "You can now import it into Autopsy for analysis."
