#!/bin/bash

# Check if the ext2 image already exists
if [ ! -f build/ext2.img ]; then
    echo "Creating ext2.img..."
    dd if=/dev/zero of=build/ext2.img bs=1M count=70  # Create a 70MB file
    mkfs.ext2 build/ext2.img  # Format it as ext2 filesystem
else
    echo "ext2.img already exists, skipping creation."
fi

# Convert the ext2.img into a C array using xxd
#xxd -i build/ext2.img > build/ext2_image.c
#echo "ext2.img converted to build/ext2_image.c"