#!/bin/sh
set -e
. ./iso.sh

echo "file isodir/boot/myos.kernel" > isodir/gdbinit
echo "target remote :1234" >> isodir/gdbinit
echo "break kernel_main" >> isodir/gdbinit
echo "c" >> isodir/gdbinit

qemu-system-$(./target-triplet-to-arch.sh $HOST) -S -s -cdrom myos.iso &
sleep 1
gdb -x isodir/gdbinit
